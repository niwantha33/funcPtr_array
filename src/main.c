#include "stdio.h"
#include "stdlib.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define OUTPUT 0x1
#define INPUT 0x0
#define INPUT_OUTPUT 0x2
#define HIGH (0x1)
#define LOW (0x0)

/* callback or function Pointer */

typedef struct
{

    uint8_t mode;
    uint8_t pinNum;
    uint8_t status;

} sensor_t;

typedef void (*funcPtr_t[2])(sensor_t pincg);

/* prototype */
__attribute__((always_inline)) static inline void pinMode(sensor_t pinConfig);
__attribute__((always_inline)) static inline void digitalWrite();
void app_main()
{

    funcPtr_t callbak = {
        pinMode,
        digitalWrite};

    sensor_t pinCg;
    pinCg.mode = OUTPUT;
    pinCg.pinNum = 0;
    pinCg.status =0;
    callbak[0](pinCg); 

    for (;;)
    {
        
        pinCg.status ^= HIGH;        
        callbak[1](pinCg);
        vTaskDelay(250 / portTICK_RATE_MS);

    }
}

__attribute__((always_inline)) static inline void pinMode(sensor_t pinConfig)
{

    gpio_config_t builtInLed = {};
    builtInLed.intr_type = GPIO_INTR_DISABLE;
    if (pinConfig.mode == OUTPUT)
        builtInLed.mode = GPIO_MODE_OUTPUT;
    if (pinConfig.mode == INPUT)
        builtInLed.mode = GPIO_MODE_INPUT;
    if (pinConfig.mode == INPUT_OUTPUT)
        builtInLed.mode = GPIO_MODE_INPUT_OUTPUT;
    builtInLed.pin_bit_mask = BIT(pinConfig.pinNum);
    builtInLed.pull_down_en = 0;
    builtInLed.pull_up_en = 0;

    gpio_config(&builtInLed);

    return;
}

__attribute__((always_inline)) static inline void digitalWrite(sensor_t pinConfig)
{
    gpio_set_level(pinConfig.pinNum, pinConfig.status);

    return;
}