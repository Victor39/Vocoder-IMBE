/*
 * TestPin.h
 *
 *  Created on: 24 ���. 2018 �.
 *      Author: Alsheuski
 */

#ifndef SOURCES_TESTPIN_H_
#define SOURCES_TESTPIN_H_

class TestPin {

public:
	static void turnOn();
	static void turnOff();
	static void startUp();

private:
	static bool m_started;
};

#endif /* SOURCES_TESTPIN_H_ */
