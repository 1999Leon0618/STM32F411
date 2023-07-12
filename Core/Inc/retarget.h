//
// Created by u10510 on 2023/7/6.
//

#ifndef WB55_PWM_PULSE_COUNT_RETARGET_H
#define WB55_PWM_PULSE_COUNT_RETARGET_H

#include "stm32wbxx_hal.h"
#include <sys/stat.h>
#include <stdio.h>

void RetargetInit(UART_HandleTypeDef *huart);

int _isatty(int fd);

int _write(int fd, char *ptr, int len);

int _close(int fd);

int _lseek(int fd, int ptr, int dir);

int _read(int fd, char *ptr, int len);

int _fstat(int fd, struct stat *st);

#endif //WB55_PWM_PULSE_COUNT_RETARGET_H
