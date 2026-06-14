6/14/2026
Function
- Implemented LED on / off using bare-metal GPIO drivers on the ESP32
Registers Used
GPIO BASE: 3FF44000
- GPIO_ENABLE_REG - 0x20
- GPIO_OUT_REG - 0x04
- GPIO_IN_REG - 0x3C
IO_MUX BASE: 0x3FF49000
- IO_MUX_4_REG - 0x48 (MCU_SEL 14-12, FUN_IE 9, FUN_WPU 8, FUN_WPD 7)

Problems Encountered
- GPIO4 Input read low regardless of pin state
- Traced to MCUSEL
- Updated to Clear --> Set framework
- Wrote 0x02 to route GPIO4 to GPIO Matrix, MCUSEL can route to various internal functions (I2C, SPI, etc.)
