#ifdef APP_UINVERTER

#include "usr/uInverter/task_uInverter.h"
#include "sys/scheduler.h"
#include "drv/pwm.h"
#include "drv/cpu_timer.h"
#include <math.h>

// Scheduler TCB which holds task "context"
static task_control_block_t tcb;

double Ts    = 1.0 / 10000.0; // [sec]
double theta = 0.0;           // [rad]
double omega = 377.0;         // [rad/s]
double Do    = 0.8;           // [--]

int task_uInverter_init(void){
    if (scheduler_tcb_is_registered(&tcb)) {
        return FAILURE;
    }

    // Fill TCB with parameters
    scheduler_tcb_init(&tcb, task_uInverter_callback, 
                        NULL, "uInverter", TASK_UINVERTER_INTERVAL_USEC);

    task_stats_enable(&tcb.stats);

    // Register task with scheduler
    return scheduler_tcb_register(&tcb);
}
int task_uInverter_deinit(void){
    return scheduler_tcb_unregister(&tcb);
}

void task_uInverter_callback(void *arg){
    // Update theta
    theta += (Ts * omega);

    // Wrap to 2*pi
    theta = fmod(theta, 2.0 * M_PI);
    
    // Calculate desired duty ratios
    double duty_a = 0.5 + Do/2.0 * cos(theta);
    double duty_b = 0.5 + Do/2.0 * cos(theta - 2.0*M_PI/3.0);
    double duty_c = 0.5 + Do/2.0 * cos(theta - 4.0*M_PI/3.0);

    // Update PWM peripheral in FPGA
    pwm_set_duty(0, duty_a); // Set HB1 duty ratio (INV1, PWM1 and PWM2)
    pwm_set_duty(1, duty_b); // Set HB2 duty ratio (INV1, PWM3 and PWM4)
    pwm_set_duty(2, duty_c); // Set HB3 duty ratio (INV1, PWM5 and PWM6)
}

int task_uInverter_set_frequency(double freq){
    omega = freq;
    return SUCCESS;
}
int task_uInverter_set_amplitude(double amplitude){

    if (amplitude < 0 || amplitude > 1)
        return FAILURE;
    Do = amplitude;
    return SUCCESS;
}

int task_uInverter_stats_print(){
	task_stats_print(&tcb.stats);
	return SUCCESS;
}

int task_uInverter_stats_reset(){
	task_stats_reset(&tcb.stats);
	return SUCCESS;
}

#endif // APP_uInverter
