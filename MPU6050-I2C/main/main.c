#include <stdio.h>
#include <soc/i2c_reg.h>
#include <soc/i2c_struct.h>
#include <driver/i2c_master.h>

void i2c_init(i2c_master_dev_handle_t *dev_handle) {
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = 22,
        .sda_io_num = 21,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x68,
        .scl_speed_hz = 100000,
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, dev_handle));
}

void i2c_write_reg(i2c_master_dev_handle_t dev_handle, uint8_t reg_address, uint8_t reg_value, uint8_t write_len) {
    uint8_t write_buf[] = {reg_address, reg_value};
    ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, write_buf, write_len, -1));
}

void i2c_read_reg(i2c_master_dev_handle_t dev_handle, uint8_t reg_address, uint8_t *read_buf, uint8_t read_len) {
    uint8_t write_buf[] = {reg_address};
    ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, write_buf, 1, read_buf, read_len, -1));
}

void app_main() {
    i2c_master_dev_handle_t dev_handle;
    i2c_init(&dev_handle);
}