#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "i2c-lcd.h"
#include "unistd.h"
#include "esp_timer.h"
#include "Date.h"
#include <time.h>
     
static const char *TAG = "i2c-simple-example";

char buffer[10];

int counter = 0 ;
int tmp;
int hour,minute,second;
CDate date;
clock_t start, end;
double cpu_time_used;
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

void Digital_Clock(void *arg);
void Digital_Clock(void *arg){
    start = clock();
	counter++;
	tmp = counter;
	hour = tmp/3600;
	tmp = tmp%3600;
	minute = tmp/60;
	tmp = tmp%60;
	second = tmp;
	
    sprintf(buffer, "%02d", hour);
    lcd_put_cur(0, 4);
    lcd_send_string(buffer);

    sprintf(buffer, "%02d", minute);
    lcd_put_cur(0, 7);
    lcd_send_string(buffer);

    sprintf(buffer, "%02d", second);
    lcd_put_cur(0, 10);
    lcd_send_string(buffer);
    
        
	sprintf(buffer, "%02d", date.day);
    lcd_put_cur(1, 3);
    lcd_send_string(buffer);

	sprintf(buffer, "%02d", date.month);
    lcd_put_cur(1, 6);
    lcd_send_string(buffer);
       
	sprintf(buffer, "%04d", date.year);
    lcd_put_cur(1, 9);
    lcd_send_string(buffer);
	   
    if (counter == 86400 ){
    	counter = 0;
    	CDate_Increment(&date);
	}
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
}	
void app_main(void)
{
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");
	
    lcd_init();
    lcd_clear();
    lcd_put_cur(0, 4);
    lcd_send_string("00:00:00");
    lcd_put_cur(1, 3);
    lcd_send_string("00/00/0000");
    date.day = 28;
   	date.month = 2;
   	date.year = 2023;
   // CDate date; 
//    Khoi tao timer
    counter = 86399;
	const esp_timer_create_args_t periodic_timer_args ={
		.callback = &Digital_Clock,
		.name = "periodic"
	};
	esp_timer_handle_t periodic_timer;
	
	esp_timer_create(&periodic_timer_args,&periodic_timer);
	while (1){
	esp_timer_start_periodic(periodic_timer,1000000 - cpu_time_used*1000 );
	}
	
}
