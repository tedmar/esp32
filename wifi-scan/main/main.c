/*
 * 	main.c
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/i2c.h"

#include "tdelay.h"
#include "oled.h"
#include "test_led.h"

#define MAX_APs 20

static wifi_ap_record_t ap_records[MAX_APs];

// From auth_mode code to string
static char*
getAuthModeName(wifi_auth_mode_t auth_mode) {
	
	char *names[] = {"OPEN", "WEP", "WPA PSK", "WPA2 PSK", "WPA WPA2 PSK", "MAX"};
	return names[auth_mode];
}

// Empty event handler
static esp_err_t
event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}


void
task_idle( void *ignore )
{
    init_test_led();
    while(1)
    {
        led_toggle();
        tdelay(1000);
    }
}

static void
init_oled_display( void )
{
	oled_init();
	oled_clear();
	oled_contrast(255);
}

#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22

void
i2c_master_init( void )
{
    i2c_config_t i2c_config =
    {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 1000000
    };
    i2c_param_config(I2C_NUM_0, &i2c_config);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}

static void
wait_next( void )
{
	int delay, subst;

	for( delay = 2000, subst = 500; delay > 50; delay -= subst )
	{
		led_toggle();
		tdelay( delay );
		led_toggle();
		tdelay( delay );
		if( delay <= 1000 )
			subst = 50;
	}
}

void
app_main( void )
{
	wifi_ap_record_t *p;
	uint16_t ap_num = MAX_APs;
	char buff[30];

	i2c_master_init();
	init_oled_display();
	init_test_led();
	// initialize NVS
	ESP_ERROR_CHECK(nvs_flash_init());
	
//	xTaskCreate(&task_idle, "task_idle", 2048, NULL, 5, NULL);

	// initialize the tcp stack
	tcpip_adapter_init();

	// initialize the wifi event handler
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
	
	// configure, initialize and start the wifi driver
	wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_start());
	
	// configure and run the scan process in blocking mode
	wifi_scan_config_t scan_config =
	{
		.ssid = 0,
		.bssid = 0,
		.channel = 0,
        .show_hidden = true
    };

	while(1)
	{
		oled_clear();
		oled_text( "Start scan" );
		printf("Start scanning...");
		ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
		printf(" completed!\n\n");
		oled_text( "Scan done" );
			
		// get the list of APs found in the last scan
		ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_num, ap_records));

		sprintf( buff, "Found %d access", ap_num );
		oled_text(buff);
		oled_text("");

		for( p = ap_records; p < ap_records + ap_num && p->rssi > -80; ++p )
			oled_text( (char *)p->ssid );

		// print the list 
		printf("Found %d access points:\n\n", ap_num);
		printf("               SSID              | Channel | RSSI |   Auth Mode \n");
		printf("----------------------------------------------------------------\n");
		for( p = ap_records; p < ap_records + ap_num; ++p )
			printf("%32s | %7d | %4d | %12s\n", (char *)p->ssid, p->primary, p->rssi, getAuthModeName(p->authmode));
		printf("----------------------------------------------------------------\n");

		wait_next();
	}

	
}

