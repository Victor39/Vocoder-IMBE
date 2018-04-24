/*
 * TestPin.cpp
 *
 *  Created on: 24 апр. 2018 г.
 *      Author: Alsheuski
 */

#include <sources/TestPin.h>
#include "gpio.h"
#include "psc.h"
#include "soc_OMAPL138.h"
#include "lcdkOMAPL138.h"

bool TestPin::m_started = false;

void TestPin::turnOn () {

	if (!m_started) {
		startUp();
	}
	GPIOPinWrite(SOC_GPIO_0_REGS, 111, GPIO_PIN_HIGH);
}

void TestPin::turnOff () {
	if (!m_started) {
		startUp();
	}
	GPIOPinWrite(SOC_GPIO_0_REGS, 111, GPIO_PIN_LOW);
}

void TestPin::startUp () {

#define SYS_BASE                                 0x01C14000
#define PINMUX13                                 *(unsigned int*)(SYS_BASE + 0x154)
#define CSL_SYSCFG_PINMUX13_PINMUX13_7_4_OBSCLK0 (0x00000001u)
#define CSL_SYSCFG_PINMUX13_PINMUX13_7_4_GP       (0x00000008u)
#define CSL_SYSCFG_PINMUX13_PINMUX13_7_4_SHIFT   (0x00000004u)

    /* The Local PSC number for GPIO is 3. GPIO belongs to PSC1 module.*/
    PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_GPIO, PSC_POWERDOMAIN_ALWAYS_ON,
    PSC_MDCTL_NEXT_ENABLE);

	// Настройка пинов GPIO (TP10)
	PINMUX13 |= ((CSL_SYSCFG_PINMUX13_PINMUX13_7_4_GP) << (CSL_SYSCFG_PINMUX13_PINMUX13_7_4_SHIFT));

    /* Sets the pin 109 (GP6[12]) as input.*/
    GPIODirModeSet(SOC_GPIO_0_REGS, 111, GPIO_DIR_OUTPUT);


	m_started = true;
}
