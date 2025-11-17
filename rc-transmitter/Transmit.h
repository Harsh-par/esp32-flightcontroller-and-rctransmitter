#pragma once

void SendJoystick(void);
void SendCallback(const uint8_t *mac_addr, esp_now_send_status_t status);
void AttemptConnection(void);
