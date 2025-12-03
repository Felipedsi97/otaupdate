#include <stdio.h>
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "cJSON.h"
#include "esp_log.h"
#include "esp_crt_bundle.h"

//if you create a local http server, you can set the json path as below
//#define UPDATE_URL "http://192.168.0.10/update_file.json"
#define UPDATE_URL "https://raw.githubusercontent.com/Felipedsi97/otaupdate/refs/heads/main/firmware_version.json"
#define VERSION_CURRENT "2.0"  // Replace with dynamic version if preferred

static const char *TAG = "OTA_JSON";

esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    return ESP_OK;
}

void perform_ota(const char *url) {
    if (strncmp(UPDATE_URL, "https://", 8) != 0) {
        ESP_LOGE("OTA", "Can't update de device, HTTP connection denied");
        return;
    }
    
    esp_http_client_config_t http_config = {
        .url = url,
        .event_handler = _http_event_handler,
        //comment two lines below and set allow ota http in menuconfig to skip https validation
        .use_global_ca_store = true,
        .crt_bundle_attach = esp_crt_bundle_attach
    };

    esp_https_ota_config_t ota_config = {
        .http_config = &http_config,
        //.partial_http_download = false,  // Optional: set true if needed
    };

    esp_err_t ret = esp_https_ota(&ota_config);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "OTA successful! Rebooting...");
        esp_restart();
    } else {
        ESP_LOGE(TAG, "OTA failed: %s", esp_err_to_name(ret));
    }
}

bool is_newer_version(const char *new_version) {
    return strcmp(new_version, VERSION_CURRENT) > 0;  // Simplified comparison
}

void check_for_update() {
    //ESP_ERROR_CHECK(esp_tls_init_global_ca_store());

    esp_http_client_config_t config = {
        .url = UPDATE_URL,
        .event_handler = _http_event_handler,
        //comment two lines below and set allow ota http in menuconfig to skip https validation
        .use_global_ca_store = true,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        ESP_LOGE(TAG, "Failed to initialize HTTP client");
        return;
    }

    // Open the HTTP connection
    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return;
    }

    int len = esp_http_client_fetch_headers(client);
    if (len <= 0) {
        ESP_LOGE(TAG, "Invalid content length: %d", len);
        esp_http_client_cleanup(client);
        return;
    }

    char *buffer = malloc(len + 1);
    if (!buffer) {
        ESP_LOGE(TAG, "Failed to allocate memory for JSON buffer");
        esp_http_client_cleanup(client);
        return;
    }

    int total_read = 0;
    int read_len = 0;
    while ((read_len = esp_http_client_read(client, buffer + total_read, len - total_read)) > 0) {
        total_read += read_len;
    }
    buffer[total_read] = '\0';

    //ESP_LOGI(TAG, "Raw JSON: %s", buffer);

    cJSON *json = cJSON_Parse(buffer);
    if (json) {
        const cJSON *ver = cJSON_GetObjectItemCaseSensitive(json, "version");
        const cJSON *url = cJSON_GetObjectItemCaseSensitive(json, "url");
        if (cJSON_IsString(ver) && cJSON_IsString(url)) {
            ESP_LOGI(TAG, "Latest version: %s", ver->valuestring);
            ESP_LOGI(TAG, "Device version: %s", VERSION_CURRENT);
            if (is_newer_version(ver->valuestring)) {
                ESP_LOGI(TAG, "New version found! Starting OTA...");
                perform_ota(url->valuestring);
            } else {
                ESP_LOGI(TAG, "Already up to date.");
            }
        }
        cJSON_Delete(json);
    }

    free(buffer);
    esp_http_client_cleanup(client);
}
