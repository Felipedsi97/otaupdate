#ifndef OTA_UPDATE_H
#define OTA_UPDATE_H

// Function declarations
void perform_ota(const char *url);
bool is_newer_version(const char *new_version);
void check_for_update(void);

#endif // OTA_UPDATE_H
