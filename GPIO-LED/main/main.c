#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h" 

void app_main(void) {
    volatile uint32_t *in = (volatile uint32_t *) 0x3FF4403C;
    volatile uint32_t *io = (volatile uint32_t *) 0x3FF49048;
    volatile uint32_t *enable = (volatile uint32_t *) 0x3FF44020;
    volatile uint32_t *out = (volatile uint32_t *) 0x3FF44004;

    // Enable GPIO2 Output (Safe because it's a 1-bit flag, |= is fine here)
    *enable |= (1 << 2);

    // Disable GPIO4 Output
    *enable &= ~(1 << 4);

    // FIX: Clear the specific configuration bits first
    *io &= ~((0x7 << 12) | (1 << 9) | (1 << 8) | (1 << 7));
    
    // FIX: Safely apply Function 2, Input Enable, and Pull-up Enable
    *io |= ((0x2 << 12) | (1 << 9) | (1 << 8));

    printf("io config (Should be 0x00002300): 0x%08lx\n", *io);

    while (1) {
        printf("in reg: 0x%08lx\n", *in);
        if ((*in & (1 << 4)) == (1 << 4)) {
            *out &= ~(1 << 2); // LED Off if button unpressed (Pulled HIGH)
        } else {
            *out |= (1 << 2);  // LED On if button pressed (GND / LOW)
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}