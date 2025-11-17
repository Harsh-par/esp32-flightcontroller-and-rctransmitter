#pragma once

struct __attribute__((packed)) ControllerData {
    int value_LJX = 0, value_LJY = 0, value_LSW = 0;
    int value_MJX = 0, value_MJY = 0, value_MSW = 0;
    int value_RJX = 0, value_RJY = 0, value_RSW = 0;
};
