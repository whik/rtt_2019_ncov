/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-07-05     wcc       the first version
 */


#include "usr_ncov.h"

struct NCOV_DATA dataChina = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "06-13 16:22"};;
struct NCOV_DATA dataGlobal = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL};

int parseData(uint8_t *str)
{
    int ret = 0;
    cJSON *root, *result_arr;
    cJSON *result, *global;
    time_t updateTime;
    struct tm *time;

    root = cJSON_Parse((const char *)str);   //创建JSON解析对象，返回JSON格式是否正确

    if (root != 0)
    {
        rt_kprintf("JSON format ok, start parse!!!\n");
        result_arr = cJSON_GetObjectItem(root, "results");
        if(result_arr->type == cJSON_Array)
        {
//            rt_kprintf("result is array\n");
            result = cJSON_GetArrayItem(result_arr, 0);
            if(result->type == cJSON_Object)
            {
//                rt_kprintf("result_arr[0] is object\n");

                /* china data parse */
                dataChina.currentConfirmedCount = cJSON_GetObjectItem(result, "currentConfirmedCount")->valueint;
                dataChina.currentConfirmedIncr = cJSON_GetObjectItem(result, "currentConfirmedIncr")->valueint;
                dataChina.confirmedCount = cJSON_GetObjectItem(result, "confirmedCount")->valueint;
                dataChina.confirmedIncr = cJSON_GetObjectItem(result, "confirmedIncr")->valueint;
                dataChina.curedCount = cJSON_GetObjectItem(result, "curedCount")->valueint;
                dataChina.curedIncr = cJSON_GetObjectItem(result, "curedIncr")->valueint;
                dataChina.deadCount = cJSON_GetObjectItem(result, "deadCount")->valueint;
                dataChina.deadIncr = cJSON_GetObjectItem(result, "deadIncr")->valueint;

                rt_kprintf("**********china ncov data**********\n");
                rt_kprintf("%-23s: %8d, %-23s: %8d\n", "currentConfirmedCount", dataChina.currentConfirmedCount, "currentConfirmedIncr", dataChina.currentConfirmedIncr);
                rt_kprintf("%-23s: %8d, %-23s: %8d\n", "confirmedCount", dataChina.confirmedCount, "confirmedIncr", dataChina.confirmedIncr);
                rt_kprintf("%-23s: %8d, %-23s: %8d\n", "curedCount", dataChina.curedCount, "curedIncr", dataChina.curedIncr);
                rt_kprintf("%-23s: %8d, %-23s: %8d\n", "deadCount", dataChina.deadCount, "deadIncr", dataChina.deadIncr);

                /* global data parse */
                global = cJSON_GetObjectItem(result, "globalStatistics");
                if(global->type == cJSON_Object)
                {
                    dataGlobal.currentConfirmedCount = cJSON_GetObjectItem(global, "currentConfirmedCount")->valueint;
                    dataGlobal.currentConfirmedIncr = cJSON_GetObjectItem(global, "currentConfirmedIncr")->valueint;
                    dataGlobal.confirmedCount = cJSON_GetObjectItem(global, "confirmedCount")->valueint;
                    dataGlobal.confirmedIncr = cJSON_GetObjectItem(global, "confirmedIncr")->valueint;
                    dataGlobal.curedCount = cJSON_GetObjectItem(global, "curedCount")->valueint;
                    dataGlobal.curedIncr = cJSON_GetObjectItem(global, "curedIncr")->valueint;
                    dataGlobal.deadCount = cJSON_GetObjectItem(global, "deadCount")->valueint;
                    dataGlobal.deadIncr = cJSON_GetObjectItem(global, "deadIncr")->valueint;

                    rt_kprintf("\n**********global ncov data**********\n");
                    rt_kprintf("%-23s: %8d, %-23s: %8d\n", "currentConfirmedCount", dataGlobal.currentConfirmedCount, "currentConfirmedIncr", dataGlobal.currentConfirmedIncr);
                    rt_kprintf("%-23s: %8d, %-23s: %8d\n", "confirmedCount", dataGlobal.confirmedCount, "confirmedIncr", dataGlobal.confirmedIncr);
                    rt_kprintf("%-23s: %8d, %-23s: %8d\n", "curedCount", dataGlobal.curedCount, "curedIncr", dataGlobal.curedIncr);
                    rt_kprintf("%-23s: %8d, %-23s: %8d\n", "deadCount", dataGlobal.deadCount, "deadIncr", dataGlobal.deadIncr);

                } else return 1;

                /* 毫秒级时间戳转字符串 */
                updateTime = (time_t )(cJSON_GetObjectItem(result, "updateTime")->valuedouble / 1000);
                updateTime += 8 * 60 * 60; /* UTC8校正 */
                time = localtime(&updateTime);
                /* 格式化时间 */
                strftime(dataChina.updateTime, 20, "%m-%d %H:%M", time);
                rt_kprintf("update: %s\r\n", dataChina.updateTime);/* 06-24 11:21 */

                gui_show_ncov_data(dataChina, dataGlobal);
            } else return 1;
        } else return 1;
        rt_kprintf("\nparse complete \n");
    }
    else
    {
        rt_kprintf("JSON format error:%s\n", cJSON_GetErrorPtr()); //输出json格式错误信息
        return 1;
    }
    cJSON_Delete(root);

    return ret;
}

int get_NCOV_Data(void)
{
    char *uri = RT_NULL;
    struct webclient_session* session = RT_NULL;
    uint8_t *buffer = RT_NULL;
    int index, ret = 0;
    int bytes_read, resp_status;
    int content_length = -1;
    int buffer_size = 1600;
    uri = web_strdup(API_NCOV);
    rt_kprintf("start get api: %s\r\n", API_NCOV);
    if(uri != RT_NULL)
    {
        buffer = (unsigned char *) web_malloc(buffer_size);
        if (buffer == RT_NULL)
        {
            rt_kprintf("no memory for receive buffer.\n");
            ret = -RT_ENOMEM;
            goto __exit;
        }

        /* create webclient session and set header response size */
        session = webclient_session_create(buffer_size);
        if (session == RT_NULL)
        {
            ret = -RT_ENOMEM;
            goto __exit;
        }

        /* send GET request by default header */
        if ((resp_status = webclient_get(session, uri)) != 200)
        {
            rt_kprintf("webclient GET request failed, response(%d) error.\n", resp_status);
            ret = -RT_ERROR;
            goto __exit;
        }

        rt_kprintf("webclient get response data: \n");

        content_length = webclient_content_length_get(session);
        if (content_length < 0)
        {
            rt_kprintf("webclient GET request type is chunked.\n");

            do
            {
                bytes_read = webclient_read(session, buffer, buffer_size);
                if (bytes_read <= 0)
                    break;

                for (index = 0; index < bytes_read; index++)
                {
                    rt_kprintf("%c", buffer[index]);
                }
            } while (1);

            rt_kprintf("\n");
        }
        else
        {
            /* 读取服务器响应的数据 */
            bytes_read = webclient_read(session, buffer, content_length);
            rt_kprintf("data length:%d\n", bytes_read);

            buffer[bytes_read] = '\0';
            rt_kprintf("parse data\r\n");
            parseData(buffer);
            rt_kprintf("\n");
        }

        __exit:
        if (session)
            webclient_close(session);

        if (buffer)
            web_free(buffer);
    }
    else
        rt_kprintf("api error: %s\n", API_NCOV);

    return ret;
}
MSH_CMD_EXPORT(get_NCOV_Data, get api weather);
