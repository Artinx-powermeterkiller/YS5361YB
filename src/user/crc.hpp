// Copyright (c) 2022 ChenJun
// Licensed under the Apache-2.0 License.

#ifndef RM_SERIAL_DRIVER__CRC_HPP_
#define RM_SERIAL_DRIVER__CRC_HPP_

#include <stdint.h>

/**
  * @brief Append CRC16 value to the end of the buffer
  * @param[in] pchMessage : Data to Verify,
  * @param[in] dwLength : Stream length = Data + checksum
  * @return none
  */
void Append_CRC16_Check_Sum(uint8_t * pchMessage, uint32_t dwLength);


#endif  // RM_SERIAL_DRIVER__CRC_HPP_
