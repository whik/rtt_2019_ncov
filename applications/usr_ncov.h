/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-05     wcc       the first version
 */
#ifndef APPLICATIONS_USR_NCOV_H_
#define APPLICATIONS_USR_NCOV_H_


#include <webclient.h>
#include <rtdevice.h>
#include <rtthread.h>
#include <cJSON.h>
#include "drv_9341.h"

#define API_NCOV     "https://lab.isaaclin.cn/nCoV/api/overall"

struct NCOV_DATA{
    int currentConfirmedCount;
    int currentConfirmedIncr;
    int confirmedCount;
    int confirmedIncr;
    int curedCount;
    int curedIncr;
    int deadCount;
    int deadIncr;
    int seriousCount;
    int seriousIncr;

    char updateTime[20];
};

extern struct NCOV_DATA dataChina;
extern struct NCOV_DATA dataGlobal;

int get_NCOV_Data(void);
int parseData(uint8_t *str);


#endif /* APPLICATIONS_USR_NCOV_H_ */
