/*
 * ImbeTester.cpp
 *
 *  Created on: 24 апр. 2018 г.
 *      Author: Alsheuski
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sources/ImbeTester.h>
#include "TestPin.h"
#include "imbe_vocoder/imbe_vocoder.h"
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/hal/Seconds.h>
#include <ti/sysbios/interfaces/ISeconds.h>
#include <time.h>
#include "wav/wavreader.h"
#include "wav/wavwriter.h"

#define IN_ENCODER_FRAME_SAMPLE_SIZE (160)
#define IN_ENCODER_FRAME_BYTE_SIZE (IN_ENCODER_FRAME_SAMPLE_SIZE*2)
#define OUT_ENCODER_FRAME_SAMPLE_SIZE (8)
#define FRAMES_QUANTITY	(3)

//const short voc_test[88] = { 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, //12 bits
//                             0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, //12 bits
//                             0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, //12 bits
//                             0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, //12 bits
//                             1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0,    //11 bits
//                             0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0,    //11 bits
//                             0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,    //11 bits
//                             0, 0, 0, 0, 0, 0, 0                 //7 bits
//        };

static const int16_t encoded_tone_frame[8] = { 0x009B, 0x0088, 0x00CC, 0x0621, 0x07B2, 0x002A, 0x004C, 0x0000 };


static const int16_t inEncoderBuffer[FRAMES_QUANTITY][IN_ENCODER_FRAME_SAMPLE_SIZE] = { {7071, 10000, 7071, 0, -7071, -10000, -7071,
		0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0,
		-7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071,
		10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000,
		-7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071,
		0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0,
		7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071,
		-10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000,
		7071, 0, -7071, -10000, -7071, 0}, {7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000,
		-7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071,
		0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0,
		7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071,
		-10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000,
		7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071,
		0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0,
		-7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0}, {
		7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071,
		-10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000,
		7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071,
		0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0,
		-7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071,
		10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000,
		-7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0, 7071, 10000, 7071,
		0, -7071, -10000, -7071, 0, 7071, 10000, 7071, 0, -7071, -10000, -7071, 0}};

int16_t outDecoderBuffer[3][IN_ENCODER_FRAME_SAMPLE_SIZE];

static const int16_t outDecoderEtalonBuffer[3][IN_ENCODER_FRAME_SAMPLE_SIZE] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1,
		-1, 0, 0, 1, 1, 0, 0, 3, 1, 0, 4, -3, -8, 0, 0, 2, 9, 0, 0, 0, 7, 7, 4, 8, -9, -9, 13, 4, 4, 5, -6, 10, 11, 5, -12, -7, 6,
		-13, 6, 20, -7, 7, 0, -15, 7, 0, 0, -8, 8, 16, -8, 16, 8, 0, 0, -16, -8, 0, -8, -16, -32, -16, 0, -24, 8, -8, 0, 16, 0, 24,
		8, 0, 0, 0, 0, 0, 8, 0, -8, 0, 8, 8, 16, 0, 0, -8, 8, 8, 8, 0, -8, 0, 8, 8, 0, 0, -8, 8, 16, 8, 8, 0, }, {-8, 8, 8, 8, -8,
		-8, -16, -8, 24, 0, 0, 8, -24, -16, -8, -8, 0, -16, -16, 0, 0, 0, 0, -16, -8, -16, 0, 8, -8, 0, 0, 16, 8, 8, 16, 8, 8, 8, 0,
		0, 0, 8, -8, 0, 16, 0, 16, 0, -8, 0, -16, -8, 0, 0, -8, -16, -1, 7, -16, 7, -16, -24, 1, -14, 13, 1, -10, 0, -2, 7, 3, -3,
		-6, -4, 1, 3, 10, 23, 11, -12, -8, -21, 7, -8, 3, 19, 6, 7, 0, -8, -6, 0, 4, -15, -1, 9, 0, 17, 0, -9, 0, 23, 7, -8, 0, 0,
		0, 0, 8, -16, 8, 0, -8, 0, 8, 0, -16, 24, 8, -8, 0, 16, 8, -8, 8, 0, 0, 16, 0, -16, -8, 16, 0, 0, 16, -16, 0, 0, -16, 8, 0,
		0, -8, -8, -24, 16, -8, -16, 0, 8, 16, 0, 0, 32, 8, 8, 16, -8, 8, -24}, {0, 8, -16, -16, 0, 0, 8, -8, 24, 0, 8, -8, -8, 8,
		0, 24, -16, -8, 8, 0, 8, -16, 8, 16, -16, 8, -8, -8, 8, 8, 8, 24, -16, 8, -8, 0, 16, -16, 8, 8, -8, 16, 16, -16, 0, 0, 8, 0,
		0, -16, 0, -8, 0, 16, -8, -8, 17, 39, 64, 34, -100, -230, -136, 78, 217, -52, -584, -678, -292, 359, 897, 1060, 690, 14,
		-555, -698, -365, 74, 401, 433, 256, 12, 4, -68, -418, -183, 156, 30, -254, -593, -388, -12, -44, -515, -273, 591, 795, 907,
		531, 41, -326, -331, -775, -1728, -1623, -1032, 648, 2008, 2136, 1832, 552, -1200, -2008, -1008, 1032, 2272, 1432, -760,
		-2648, -2808, -528, 1872, 2408, 1672, -56, -1344, -1304, -688, -176, 336, 720, 800, 440, -8, 0, 160, 336, -336, -792, -1088,
		-1112, -48, 1072, 1600, 816, -280, -1728, -1712, 544, 1568, 848, -704, -2176, -2216, -936, 896, 2120, 1944, 1440, 744}};

uint16_t resultBuffer[FRAMES_QUANTITY][OUT_ENCODER_FRAME_SAMPLE_SIZE];

static const uint16_t outEncoderEtalonBuffer[FRAMES_QUANTITY][OUT_ENCODER_FRAME_SAMPLE_SIZE] = { {1877, 4032, 3823, 2050, 0, 19,
		1262, 124}, {18, 255, 4031, 2553, 62, 512, 70, 121}, {178, 1946, 3231, 1329, 95, 1834, 26, 37}};

void ImbeTester::testExecutionTime () {

    TestPin::startUp();

	ISeconds_Time ts1, ts2;
	ISeconds_Time encodeExecutionTime, decodeExecutionTime;

	imbe_vocoder imbeEncoder;
	imbe_vocoder imbeDecoder;
	uint16_t frameIndex = 0;
	while (true) {
		frameIndex = (++frameIndex) % FRAMES_QUANTITY;

		Seconds_getTime(&ts1);
		TestPin::turnOn();
		imbeEncoder.imbe_encode((int16_t*) (&resultBuffer[frameIndex][0]), (int16_t*) &inEncoderBuffer[frameIndex][0]);
		TestPin::turnOff();
		Seconds_getTime(&ts2);
		encodeExecutionTime.secs = (ts2.secs - ts1.secs);
		encodeExecutionTime.nsecs = (ts2.nsecs - ts1.nsecs);

		Task_sleep(10);

		Seconds_getTime(&ts1);
		TestPin::turnOn();
		imbeDecoder.imbe_decode((int16_t*) (&resultBuffer[frameIndex][0]), &outDecoderBuffer[frameIndex][0]);
		TestPin::turnOff();
		Seconds_getTime(&ts2);
		decodeExecutionTime.secs = (ts2.secs - ts1.secs);
		decodeExecutionTime.nsecs = (ts2.nsecs - ts1.nsecs);

		Task_sleep(2000);

		printf("Coding time is   %d msec! \n", encodeExecutionTime.nsecs/1000000);
		printf("Decoding time is   %d msec! \n", decodeExecutionTime.nsecs/1000000);
	}
}

bool ImbeTester::testCorrectnessOfWork () {

	memset(resultBuffer, 0xff, OUT_ENCODER_FRAME_SAMPLE_SIZE * FRAMES_QUANTITY);

	imbe_vocoder imbeEncoder;
	imbe_vocoder imbeDecoder;
	bool encodedSuccessfully = true;
	bool decodedSuccessfully = true;
	for (uint16_t frameIndex = 0; frameIndex < FRAMES_QUANTITY; frameIndex++) {


		imbeEncoder.imbe_encode((int16_t*) (&resultBuffer[frameIndex][0]), (int16_t*) &inEncoderBuffer[frameIndex][0]);
		int encodeErrors = 0;
		for (int sampleIndex = 0; sampleIndex < OUT_ENCODER_FRAME_SAMPLE_SIZE; sampleIndex++) {
			if (resultBuffer[frameIndex][sampleIndex] != outEncoderEtalonBuffer[frameIndex][sampleIndex])
				encodeErrors++;
		}
		encodedSuccessfully = ((encodeErrors == 0) ? true : false) && encodedSuccessfully;

		Task_sleep(10);

		imbeDecoder.imbe_decode((int16_t*) (&resultBuffer[frameIndex][0]), &outDecoderBuffer[frameIndex][0]);
		int decodeErrors = 0;
		for (int sampleIndex = 0; sampleIndex < IN_ENCODER_FRAME_SAMPLE_SIZE; sampleIndex++) {
			if (resultBuffer[frameIndex][sampleIndex] != outDecoderEtalonBuffer[frameIndex][sampleIndex])
				encodeErrors++;
		}
		decodedSuccessfully = ((decodeErrors == 0) ? true : false) && decodedSuccessfully;

		Task_sleep(10);
	}

	if( (encodedSuccessfully && decodedSuccessfully) ) {
		printf("Encoding and decoding completed SUCCESSFULLY! \n");
	}
	else {
		printf("Encoding and decoding completed with ERORRS! \n");
	}

	return (encodedSuccessfully && decodedSuccessfully);
}

void ImbeTester::testTonePatternDecoding () {

	const uint16_t framesQuantity = 10;
	int16_t outToneBuffer[framesQuantity][IN_ENCODER_FRAME_SAMPLE_SIZE];

	imbe_vocoder imbeDecoder;
	for (int frameIndex = 0; frameIndex < framesQuantity; frameIndex++) {
		imbeDecoder.imbe_decode((int16_t*) (encoded_tone_frame), &outToneBuffer[frameIndex][0]);
	}

	asm(" nop");
}

void ImbeTester::testAudioFile () {

	WavReader * pWavReader = WavReader::make("d:\\Projects\\Vocoder_IMBE\\voice_8kHz.wav");
	WavWriter * pWavWriter = WavWriter::make("d:\\Projects\\Vocoder_IMBE\\imbe_voice_8kHz_9.wav");
	pWavWriter->addHeader(pWavReader->getHeader());

	int writeCounter = 0;

	int16_t inEncoderBuffer[IN_ENCODER_FRAME_SAMPLE_SIZE];
	int16_t outDecoderBuffer[IN_ENCODER_FRAME_SAMPLE_SIZE];
	uint8_t outEncoderBuffer[OUT_ENCODER_FRAME_SAMPLE_SIZE*2];
	memset(outEncoderBuffer, 0xff, OUT_ENCODER_FRAME_SAMPLE_SIZE*2);
	imbe_vocoder imbeEncoder;
	imbe_vocoder imbeDecoder;
	while(1) {
		uint16_t byteRead = pWavReader->readBytes((uint8_t*)inEncoderBuffer, IN_ENCODER_FRAME_BYTE_SIZE);
		if(byteRead == IN_ENCODER_FRAME_BYTE_SIZE) {
			imbeEncoder.imbe_encode((int16_t*)outEncoderBuffer, inEncoderBuffer);
			imbeDecoder.imbe_decode((int16_t*)outEncoderBuffer, outDecoderBuffer);
			pWavWriter->writeBytes((uint8_t*)outDecoderBuffer, IN_ENCODER_FRAME_BYTE_SIZE);
			writeCounter += IN_ENCODER_FRAME_BYTE_SIZE;
			printf("Recorded %d bytes \n", writeCounter);
		}
		else if(byteRead>0){
			pWavWriter->writeBytes((uint8_t*)inEncoderBuffer, byteRead);
			writeCounter += byteRead;
			printf("Recorded %d bytes \n", writeCounter);
		}
		else {
			break;
		}
	}
	printf("Write to file is complete! \n");

	delete pWavWriter;
	delete pWavReader;
}


//static void pack(uint16_t * vocoder_frame) {
//
//	uint8_t O[11];
//
//	O[0] = (((vocoder_frame[0] / 16) & 240) + (vocoder_frame[1] / 256));
//	O[1] = (((vocoder_frame[2] / 16) & 240) + (vocoder_frame[3] / 256));
//	O[2] = (((vocoder_frame[4] / 8) & 224) + ((vocoder_frame[5] / 64) & 28) + (vocoder_frame[6] / 512));
//	O[3] = (((vocoder_frame[6] / 2) & 128) + vocoder_frame[7]);
//	O[4] = (vocoder_frame[0] & 255);
//	O[5] = (vocoder_frame[1] & 255);
//	O[6] = (vocoder_frame[2] & 255);
//	O[7] = (vocoder_frame[3] & 255);
//	O[8] = (vocoder_frame[4] & 255);
//	O[9] = (vocoder_frame[5] & 255);
//	O[10] = (vocoder_frame[6] & 255);
//}
