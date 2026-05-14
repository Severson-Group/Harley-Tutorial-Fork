#ifndef TASK_UINVERTER_H
#define TASK_UINVERTER_H

#include "sys/scheduler.h"

#define TASK_UINVERTER_UPDATES_PER_SEC (10000)
#define TASK_UINVERTER_INTERVAL_USEC   (USEC_IN_SEC / TASK_UINVERTER_UPDATES_PER_SEC)

int task_uInverter_init(void);
int task_uInverter_deinit(void);

void task_uInverter_callback(void *arg);

int task_uInverter_set_frequency(double freq);
int task_uInverter_set_amplitude(double amplitude);

int task_uInverter_stats_print();
int task_uInverter_stats_reset();

#endif // TASK_UINVERTER_H
