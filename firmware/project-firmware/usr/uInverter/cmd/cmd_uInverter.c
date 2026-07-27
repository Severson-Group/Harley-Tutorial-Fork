#ifdef APP_UINVERTER

#include "usr/uInverter/cmd/cmd_uInverter.h"
#include "sys/commands.h"
#include "sys/defines.h"
#include "sys/util.h"
#include "usr/uInverter/task_uInverter.h"
#include "drv/pwm.h"
#include <stdlib.h>
#include <string.h>

// Stores command entry for command system module
static command_entry_t cmd_entry;

// Defines help content displayed for this command
// when user types "help" at command prompt
static command_help_t cmd_help[] = {
	{ "stats <print/reset>", "Print or Reset statistics"},
    { "start", "Begin VSI output" },
    { "stop", "End VSI output" },
    { "freq <freq>", "Set the uInverter frequency" },
    { "amplitude <amp>", "Set amplitude of voltage output (0.0 to 1.0)" },
};

void cmd_uInverter_register(void)
{
    commands_cmd_init(&cmd_entry, "uInverter", "Controller commands", 
                        cmd_help, ARRAY_SIZE(cmd_help), cmd_uInverter);
    commands_cmd_register(&cmd_entry);
}

int cmd_uInverter(int argc, char **argv)
{
    if (argc == 2 && STREQ("start", argv[1])) {
        if (task_uInverter_init() != SUCCESS) {
            return CMD_FAILURE;
        }
        if (pwm_enable() != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 2 && STREQ("stop", argv[1])) {
        if (task_uInverter_deinit() != SUCCESS) {
            return CMD_FAILURE;
        }
        if (pwm_disable() != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("freq", argv[1])) {
        double new_freq = strtod(argv[2], NULL);

        if (task_uInverter_set_frequency(new_freq) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("amplitude", argv[1])) {
        double new_amplitude = strtod(argv[2], NULL);

        if (task_uInverter_set_amplitude(new_amplitude) != SUCCESS) {
            return CMD_FAILURE;
        }

        return CMD_SUCCESS;
    }

    if (argc == 3 && STREQ("stats", argv[1])){
    	if (STREQ("print", argv[2]))
    		return task_uInverter_stats_print() == SUCCESS ? CMD_SUCCESS : CMD_FAILURE;
    	if(STREQ("reset", argv[2]))
    		return task_uInverter_stats_reset() == SUCCESS ? CMD_SUCCESS : CMD_FAILURE;
    }


    return CMD_INVALID_ARGUMENTS;
}

#endif // APP_uInverter
