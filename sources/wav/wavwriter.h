#ifndef WAVWRITER_H_
#define WAVWRITER_H_

#include "wavreader.h"

class WavWriter {
public:

	virtual ~WavWriter ();
	static WavWriter * make(const char *_fname);
	void addHeader(wav_hdr);
	void writeBytes (uint8_t * buffer, const uint16_t bufferSize);
private:
	WavWriter (FILE* wavFile);
	FILE* m_wavFile;
	wav_hdr m_header;
};

#endif /* WAVWRITER_H_ */
