#include "wavwriter.h"

WavWriter::~WavWriter () {

	fclose(m_wavFile);
}

WavWriter* WavWriter::make (const char* _fname) {

	FILE * wavFile = fopen(_fname, "wb");
	if (wavFile == NULL) {
		return NULL;
	}
	else {
		return new WavWriter(wavFile);
	}
}

void WavWriter::addHeader (wav_hdr wavHeader) {
	m_header = wavHeader;
	size_t bytesWrite = fwrite(&m_header, 1, sizeof(wav_hdr), m_wavFile);
}

void WavWriter::writeBytes (uint8_t* buffer, const uint16_t bufferSize) {

	size_t bytesWrite = fwrite(buffer, 1, bufferSize, m_wavFile);
}

WavWriter::WavWriter (FILE* wavFile) {

	m_wavFile = wavFile;
}
