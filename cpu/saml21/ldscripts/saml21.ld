/*
 * Copyright (C) 2019 ML!PA Consulting GmbH
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @addtogroup      cpu_saml21
 * @{
 *
 * @file
 * @brief           Memory definitions for Microchip SAML21
 *
 * @author          Benjamin Valentin <benjamin.valentin@ml-pa.com>
 *
 * @}
 */

ccmram_length = DEFINED( _lpsram_len ) ? _lpsram_len : 0x0 ;

MEMORY
{
    lp-sram  : ORIGIN = 0x30000000, LENGTH = _lpsram_len
}

INCLUDE cortexm.ld

SECTIONS
{
    _sbackup_data_load = LOADADDR(.backup.data);
    .backup.data :  ALIGN(4) {
        _sbackup_data = .;
        *(.backup.data)
        _ebackup_data = .;
        /* Round size so that we can use 4 byte copy in init */
        . = ALIGN(4);
    } > lp-sram AT> rom

    .backup.bss (NOLOAD) : ALIGN(4) {
        _sbackup_bss = .;
        *(.backup.bss)
        _ebackup_bss = .;
        /* Round size so that we can use 4 byte copy in init */
        . = ALIGN(4);
    } > lp-sram

    .backup.noinit (NOLOAD) : {
        _sbackup_noinit = .;
        *(.backup.noinit)
        _ebackup_noinit = .;
    } > lp-sram
}
