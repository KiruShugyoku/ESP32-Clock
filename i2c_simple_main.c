#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "i2c-lcd.h"
#include "unistd.h"

static const char *TAG = "i2c-simple-example";

char buffer[3];
/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_NUM_0;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}

void display_time_lcd(int hour, int minute, int second)
{
    sprintf(buffer, "%02d", hour);
    lcd_put_cur(0, 4);
    lcd_send_string(buffer);

    sprintf(buffer, "%02d", minute);
    lcd_put_cur(0, 7);
    lcd_send_string(buffer);

    sprintf(buffer, "%02d", second);
    lcd_put_cur(0, 10);
    lcd_send_string(buffer);
}

void app_main(void)
{
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

    lcd_init();
    lcd_clear();
    while (1)
    {
        lcd_clear();
        lcd_put_cur(0, 4);
        lcd_send_string("00:00:00");
        int hour = 0;
        int minute = 0;
        int second = 0;

        for (hour = 0; hour < 24; hour++)
        {
            for (minute = 0; minute < 60; minute++)
            {
                for (second = 0; second < 60; second++)
                {
                    display_time_lcd(hour, minute, second);
                }
            }
        }
    }
}
