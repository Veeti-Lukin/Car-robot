#include <iostream>
#include "basic-http-server/inc/http_server/TCPServer.h"
#include "BCM2711_GPIO_DRIVER/inc/BCM2711_GPIO_Driver/bcm2711_gpio_driver.h"

int main() {
    BCM2711_GPIO_DRIVER::initialize();
    BCM2711_GPIO_DRIVER::terminate();

    return 0;
}