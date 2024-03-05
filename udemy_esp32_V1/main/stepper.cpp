#include "FastLED.h"
#include "FX.h"
#include "aws_connection.h"

void initialize_base_task(void* pvParameters)
{
	gpio_pad_select_gpio(GPIO_NUM_14);
	gpio_set_direction(GPIO_NUM_14, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(GPIO_NUM_26);
	gpio_set_direction(GPIO_NUM_26, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(GPIO_NUM_27);
	gpio_set_direction(GPIO_NUM_27, GPIO_MODE_OUTPUT);
    gpio_config_t button_conf = {
        .pin_bit_mask = (1ULL << GPIO_NUM_13),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };
    gpio_config(&button_conf);
	int endstop = 1;
	based = false;
	while(1)
	{
		if(!based)
		{
			gpio_set_level(GPIO_NUM_27, 0);
			endstop = gpio_get_level(GPIO_NUM_13);
			if (blind_state == 10007)
			{
				gpio_set_level(GPIO_NUM_27, 1);
				gpio_set_level(GPIO_NUM_26, 0);
				int i = 60;
				while (endstop == 1)
				{
					if(i > 10)i--;
					gpio_set_level(GPIO_NUM_14, 1);
					vTaskDelay(i / portTICK_PERIOD_MS);
					gpio_set_level(GPIO_NUM_14, 0);
					vTaskDelay(i / portTICK_PERIOD_MS);
					endstop = gpio_get_level(GPIO_NUM_13);
				}
				gpio_set_level(GPIO_NUM_27, 0);
				blind_state = 0;
				blind_actual_state = 0;
				based = true;
			}
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void initialize_move_stepper_task(void* pvParameters)
{
	while(1)
	{
		if((blind_actual_state != blind_state) && based && blind_state <= 6000 && blind_state >= 0)
		{
			gpio_set_level(GPIO_NUM_27, 0);
			if(blind_actual_state > blind_state)
			{
				gpio_set_level(GPIO_NUM_26, 0);
				gpio_set_level(GPIO_NUM_27, 1);
			}
			else
			{
				gpio_set_level(GPIO_NUM_26, 1);
				gpio_set_level(GPIO_NUM_27, 1);
			}

			int i = 60;
			while (blind_actual_state > blind_state)
			{
				if(i > 10)i--;
				gpio_set_level(GPIO_NUM_14, 1);
				vTaskDelay(pdMS_TO_TICKS(i));
				gpio_set_level(GPIO_NUM_14, 0);
				vTaskDelay(pdMS_TO_TICKS(i));
				blind_actual_state--;
			}
			while (blind_actual_state < blind_state)
			{
				if(i > 10)i--;
				gpio_set_level(GPIO_NUM_14, 1);
				vTaskDelay(pdMS_TO_TICKS(i));
				gpio_set_level(GPIO_NUM_14, 0);
				vTaskDelay(pdMS_TO_TICKS(i));
				blind_actual_state++;
			}
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void init_base()
{
    xTaskCreate(initialize_base_task, "init base Task", 4096, NULL, 5, NULL);
}
void move_stepper()
{
    xTaskCreate(initialize_move_stepper_task, "start stepper Task", 4096, NULL, 4, NULL);
}
