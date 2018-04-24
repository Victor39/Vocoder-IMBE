/*
 *  ======== main.c ========
 */

#ifdef __cplusplus
extern "C"
{
#endif
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Types.h>
#include <ti/sysbios/knl/Task.h>
#include "../Debug/configPkg/package/cfg/app_pe674.h"

#ifdef __cplusplus
}
#endif
#include "ImbeTester.h"


extern "C" Void taskFxn (UArg a0, UArg a1)
{

	ImbeTester::testExecutionTime();
//	ImbeTester::testCorrectnessOfWork();
//	ImbeTester::testTonePatternDecoding();
//	ImbeTester::testAudioFile();
}


/*
 *  ======== main ========
 */
Int main()
{
    /*
     * use ROV->SysMin to view the characters in the circular buffer
     */
    System_printf("enter main()\n");

    BIOS_start(); /* does not return */
    return (0);
}






