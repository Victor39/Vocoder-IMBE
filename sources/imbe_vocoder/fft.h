#ifndef IMBE_VOCODER_FFT_H_
#define IMBE_VOCODER_FFT_H_

#include "imbe.h"

#define FFT_LENGTH (256)
class Fft {
public:
	Fft () {

	}
	virtual ~Fft () {

	}

	virtual void directTransform (Cmplx16 * data) const = 0;
	virtual void inverseTransform (Cmplx16 * data) const = 0;
};

#endif /* IMBE_VOCODER_FFT_H_ */
