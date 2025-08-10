#include "sd_card.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

namespace sd_card {

static const char *TAG = "sd_card";

static sdmmc_card_t *_card = nullptr;
constexpr static const char MOUNT_POINT[] = "/sdcard";

std::string_view get_base_path() {
    return MOUNT_POINT;
}

int mount(uint8_t spi_port, int spi_cs) {
    esp_err_t ret;
    
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024,
        .disk_status_check_enable = false,
        .use_one_fat = false,
    };

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = spi_port;

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = (gpio_num_t )spi_cs;
    slot_config.host_id = (spi_host_device_t )host.slot;

    ret = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &_card);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize the card (%s). ", esp_err_to_name(ret));
        return ret;
    }
    ESP_LOGI(TAG, "Filesystem mounted");

    sdmmc_card_print_info(stdout, _card);
    return ret;
}

int unmount() {
    if (_card == nullptr) {
        return -1;
    }
    return esp_vfs_fat_sdcard_unmount(MOUNT_POINT, _card);
}

int format() {
    if (_card == nullptr) {
        return -1;
    }
    return esp_vfs_fat_sdcard_format(MOUNT_POINT, _card);
}

}
