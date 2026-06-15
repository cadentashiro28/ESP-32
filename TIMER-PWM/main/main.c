#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h" 

// Register Base Addresses
#define TIMG_BASE           0x3FF5F000
#define GPIO_BASE           0x3FF44000

// Register Addresses
#define TIMG0_CONFIG        0x00
#define TIMG0_HIGH32        0x08
#define TIMG0_LOW32         0x04
#define TIMG0_T0LOADLO_REG  0x18
#define TIMG0_T0LOADHI_REG  0x1C
#define TIMG0_T0UPDATE_REG  0xC

#define GPIO_ENABLE_REG     0x20
#define GPIO_OUT_REG        0x04
    
// Register Bit Shifts    
#define TIMG0_T0_EN         31
#define TIMG0_T0_INCREASE   30
#define TIMG0_T0_DIVIDER    13

void reset_timg0(volatile uint32_t * timg0_t0loadlo_reg,
                 volatile uint32_t * timg0_t0loadhi_reg,
                 volatile uint32_t * timg0_t0update_reg) {
    // Set counters to 0
    *timg0_t0loadlo_reg &= 0;
    *timg0_t0loadhi_reg &= 0;

    // Push set counters
    *timg0_t0update_reg |= 1;
}

void init_timg0(volatile uint32_t * timg0_config_reg,
                volatile uint32_t * timg0_t0loadlo_reg,
                volatile uint32_t * timg0_t0loadhi_reg,
                volatile uint32_t * timg0_t0update_reg) {
    // Disable timer
    *timg0_config_reg &= ~(1 << TIMG0_T0_EN);

    // Set TIMG0_T0_DIVIDER
    // 80 MHz / 80, 1 / 10^6 = 1us, for 1ms: 1000 ticks
    *timg0_config_reg &= ~(0xFFFF << TIMG0_T0_DIVIDER);
    *timg0_config_reg |= (0x50 << TIMG0_T0_DIVIDER);

    // Set TIMG0 To Increase
    *timg0_config_reg |= (1 << TIMG0_T0_INCREASE);

    // Enable timer
    *timg0_config_reg |= (1 << TIMG0_T0_EN);

    reset_timg0(timg0_t0loadlo_reg, timg0_t0loadhi_reg, timg0_t0update_reg);
}

void app_main(void) {
    // Set Timer Register Pointers
    volatile uint32_t *timg0_config_reg =   (volatile uint32_t *) (TIMG_BASE + TIMG0_CONFIG);
    volatile uint32_t *timg0_t0loadlo_reg = (volatile uint32_t *) (TIMG_BASE + TIMG0_T0LOADLO_REG);
    volatile uint32_t *timg0_t0loadhi_reg = (volatile uint32_t *) (TIMG_BASE + TIMG0_T0LOADHI_REG);
    volatile uint32_t *timg0_t0update_reg = (volatile uint32_t *) (TIMG_BASE + TIMG0_T0UPDATE_REG);
    volatile uint32_t *timg0_high32_reg =   (volatile uint32_t *) (TIMG_BASE + TIMG0_HIGH32);
    volatile uint32_t *timg0_low32_reg =    (volatile uint32_t *) (TIMG_BASE + TIMG0_LOW32);

    // Set LED Register Pointers
    volatile uint32_t *gpio_enable_reg =    (volatile uint32_t *) (GPIO_BASE + GPIO_ENABLE_REG);
    volatile uint32_t *gpio_out_reg =       (volatile uint32_t *) (GPIO_BASE + GPIO_OUT_REG);


    init_timg0(timg0_config_reg, 
               timg0_t0loadlo_reg,
               timg0_t0loadhi_reg,
               timg0_t0update_reg);

    // init GPIO
    *gpio_enable_reg |= (1 << 2);

    while (1) {
        if (*timg0_low32_reg >= 0xF4240 && *timg0_low32_reg < 0x1E8480) {
            *gpio_out_reg |=  (1 << 2);
        } else if (*timg0_low32_reg >= 0x1E8480) {
            reset_timg0(timg0_t0loadlo_reg, timg0_t0loadhi_reg, timg0_t0update_reg);
        }
        else {
            *gpio_out_reg &= ~(1 << 2);
        }
        vTaskDelay(0);
    }
}