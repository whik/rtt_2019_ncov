#include <rtthread.h>
#include <board.h>
#include <rtdevice.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include <string.h>

#include "arpa/inet.h"
#include "netdev.h"
#include "ntp.h"

#include "usr_ncov.h"
#include "drv_9341.h"
#include "drv_9341_gui.h"

#define LED_RED_PIN     GET_PIN(A, 7)
#define LED_BLUE_PIN    GET_PIN(A, 6)

int main(void)
{
    int count = 1;

    rt_pin_mode(LED_RED_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_BLUE_PIN, PIN_MODE_OUTPUT);

    lcd_init(BLACK);
    gui_show_ncov_data(dataChina, dataGlobal);

    while (count++)
    {
        rt_pin_write(LED_BLUE_PIN, PIN_LOW);
        rt_pin_write(LED_RED_PIN, PIN_LOW);
        rt_thread_mdelay(100);

        rt_pin_write(LED_BLUE_PIN, PIN_HIGH);
        rt_pin_write(LED_RED_PIN, PIN_HIGH);
        rt_thread_mdelay(100);
    }

    return RT_EOK;
}
