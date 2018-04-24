#include "wavreader.h"

WavReader* WavReader::make (const char* _fname) {

	FILE * wavFile = fopen(_fname, "rb");
	if (wavFile == 0) {
		return 0;
	}
	else {
		return new WavReader(wavFile);
	}
}

WavReader::~WavReader () {

	fclose(m_wavFile);
}

wav_hdr WavReader::getHeader () {

	return m_header;
}

uint16_t WavReader::readBytes (uint8_t* buffer, const uint16_t bufferSize) {

	uint16_t bytesRead = fread(buffer, sizeof buffer[0], bufferSize / (sizeof buffer[0]), m_wavFile);
	return bytesRead;
}

WavReader::WavReader (FILE* wavFile) {

	m_wavFile = wavFile;
	size_t bytesRead = fread(&m_header, 1, sizeof(wav_hdr), m_wavFile);
    printf("Data size                  : %d \n", m_header.ChunkSize);
    printf("Sampling Rate              : %d \n", m_header.SamplesPerSec);
    printf("Number of bits used        : %d \n", m_header.bitsPerSample);
    printf("Number of channels         : %d \n", m_header.NumOfChan);
    printf("Number of bytes per second : %d \n", m_header.bytesPerSec);
}
