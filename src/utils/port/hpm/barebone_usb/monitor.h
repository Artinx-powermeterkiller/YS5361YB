/*
 * Copyright (c) 2022-2024 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __MONITOR_H
#define __MONITOR_H

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#include "monitor_kconfig.h"

int monitor_init(void);

void monitor_handle(void);


#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif //__MONITOR_H