#ifdef APP_UINVERTER

#include "usr/uInverter/app_uInverter.h"
#include "usr/uInverter/task_uInverter.h"
#include "usr/uInverter/cmd/cmd_uInverter.h"

void app_uInverter_init(void)
{
//init code here, run sub-files and their init code.
// Register "uInverter" command with system
    cmd_uInverter_register();
}

#endif // APP_uInverter
