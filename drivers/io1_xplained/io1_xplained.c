/*
 * Copyright (C) 2016 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_io1_xplained
 * @{
 *
 * @file
 * @brief       Device driver implementation for the Atmel IO1 Xplained extension
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 *
 * @}
 */

#include "io1_xplained.h"
#include "io1_xplained_internals.h"
#include "at30tse75x.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

#define ENABLE_DEBUG (0)
#include "debug.h"

/*---------------------------------------------------------------------------*
 *                          IO1 Xplained Core API                            *
 *---------------------------------------------------------------------------*/

int io1_xplained_init(io1_xplained_t *dev, const io1_xplained_params_t *params)
{
    dev->params = *params;

    /* Initialize I2C interface */
    if (at30tse75x_init(&dev->temp,
                        I2C_DEV(0),
                        I2C_SPEED_NORMAL,
                        (TEMPERATURE_BASE_ADDR | dev->params.addr)) < 0) {
        DEBUG("[io1_xplained] Cannot initialize temperature sensor.\n");
        return -IO1_XPLAINED_NOTEMP;
    }

    /* Use maximum resolution */
    at30tse75x_set_resolution(&dev->temp, AT30TSE75X_RESOLUTION_12BIT);

    if (gpio_init(IO1_LED_PIN, GPIO_OUT) < 0) {
        DEBUG("[io1_xplained] LED initialization failed\n");
        return -IO1_XPLAINED_NOLED;
    }

    if (gpio_init(IO1_GPIO1_PIN, GPIO_OUT) < 0) {
        DEBUG("[io1_xplained] GPIO1 initialization failed\n");
        return -IO1_XPLAINED_NOGPIO1;
    }

    if (gpio_init(IO1_GPIO2_PIN, GPIO_OUT) < 0) {
        DEBUG("[io1_xplained] GPIO2 initialization failed\n");
        return -IO1_XPLAINED_NOGPIO2;
    }

    DEBUG("IO1 Xplained extension initialized with success!\n");

    return IO1_XPLAINED_OK;
}
