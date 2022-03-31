#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/rmt.h>
#include <stdbool.h>
#include "sdkconfig.h"

static char tag[] = "rmt_tests";
static void dumpStatus(rmt_channel_t channel) {
	bool loop_en;
	uint8_t div_cnt;
	uint8_t memNum;
	bool lowPowerMode;
	rmt_mem_owner_t owner;
	uint16_t idleThreshold;
	uint32_t status;
	rmt_source_clk_t srcClk;

	rmt_get_tx_loop_mode(channel, &loop_en);
	rmt_get_clk_div(channel, &div_cnt);
	rmt_get_mem_block_num(channel, &memNum);
	rmt_get_mem_pd(channel, &lowPowerMode);
	rmt_get_memory_owner(channel, &owner);
	rmt_get_rx_idle_thresh(channel, &idleThreshold);
	rmt_get_status(channel, &status);
	rmt_get_source_clk(channel, &srcClk);

	ESP_LOGI(tag, "Status for RMT channel %d", channel);
	ESP_LOGI(tag, "- Loop enabled: %d", loop_en);
	ESP_LOGI(tag, "- Clock divisor: %d", div_cnt);
	ESP_LOGI(tag, "- Number of memory blocks: %d", memNum);
	ESP_LOGI(tag, "- Low power mode: %d", lowPowerMode);
	ESP_LOGI(tag, "- Memory owner: %s", owner==RMT_MEM_OWNER_TX?"TX":"RX");
	ESP_LOGI(tag, "- Idle threshold: %d", idleThreshold);
	ESP_LOGI(tag, "- Status: %d", status);
	ESP_LOGI(tag, "- Source clock: %s", srcClk==RMT_BASECLK_APB?"APB (80MHz)":"1MHz");
}

void runRmtTest() {
	ESP_LOGI(tag, ">> runRmtTest");

	rmt_config_t config;
	config.rmt_mode = RMT_MODE_TX;
	config.channel = RMT_CHANNEL_0;
	config.gpio_num = 21;
	config.mem_block_num = 1;
	config.tx_config.loop_en = 0;
	config.tx_config.carrier_en = 0;
	config.tx_config.idle_output_en = 1;
	config.tx_config.idle_level = 0;
	config.tx_config.carrier_duty_percent = 50;
	config.tx_config.carrier_freq_hz = 10000;
	config.tx_config.carrier_level = 1;
	config.clk_div = 1;

	ESP_ERROR_CHECK(rmt_config(&config));
	ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));
	config.channel = RMT_CHANNEL_1;
	config.gpio_num = 22;
	ESP_ERROR_CHECK(rmt_config(&config));
	ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));
	dumpStatus(config.channel);

	rmt_item32_t items_0[3];
	items_0[0].duration0 = 590;
	items_0[0].level0 = 0;
	items_0[0].duration1 = 22;
	items_0[0].level1 = 0;
	items_0[1].duration0 = 5;
	items_0[1].level0 = 1;
	items_0[1].duration1 = 10;
	items_0[1].level1 = 0;
	items_0[2].duration0 = 0;
	items_0[2].level0 = 0;
	items_0[2].duration1 = 0;
	items_0[2].level1 = 0;

	rmt_item32_t items_1[3];
	items_1[0].duration0 = 22;
	items_1[0].level0 = 1;
	items_1[0].duration1 = 22;
	items_1[0].level1 = 0;
	items_1[1].duration0 = 22;
	items_1[1].level0 = 0;
	items_1[1].duration1 = 22;
	items_1[1].level1 = 0;
	items_1[2].duration0 = 0;
	items_1[2].level0 = 0;
	items_1[2].duration1 = 0;
	items_1[2].level1 = 0;

	while(1) {
	ESP_ERROR_CHECK(rmt_write_items(RMT_CHANNEL_0, items_0,
			3, /* Number of items_0 */
			0 /* NOT wait till done */));
    __asm__ __volatile__ ("nop");
    __asm__ __volatile__ ("nop");
    __asm__ __volatile__ ("nop");
    __asm__ __volatile__ ("nop");
    __asm__ __volatile__ ("nop");
    __asm__ __volatile__ ("nop");
    __asm__ __volatile__ ("nop");
    __asm__ __volatile__ ("nop");
    __asm__ __volatile__ ("nop");

	ESP_ERROR_CHECK(rmt_write_items(RMT_CHANNEL_1, items_1,
			3, /* Number of items_1 */
			0 /* wait till done */));

		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
	ESP_LOGI(tag, "<< runRmtTest");
}

int app_main(){
	runRmtTest();
	return 0;
}