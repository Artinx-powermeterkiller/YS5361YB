#include "DJIMotorCANManager.hpp"
#include "GM6020.hpp"
#include "M2006.hpp"
#include "M3508.hpp"

/**
 * @brief  Check registered motor, update sensor(update current, speed and position).=
 */
void DJIMotorCANManager::ReceiveUpdate()
{
	for (uint8_t i = 0; i < 7; i++)
	{
		for (uint8_t j = 0; j < 4; j++)
		{
			if (m_GM6020[j][i] != nullptr)
			{
				m_GM6020[j][i]->SensorUpdate();
			}

			if (m_M2006[j][i] != nullptr)
			{
				m_M2006[j][i]->SensorUpdate();
			}

			if (m_M3508[j][i] != nullptr)
			{
				m_M3508[j][i]->SensorUpdate();
			}
		}
	}
}

void DJIMotorCANManager::SendUpdate()
{
	for (uint8_t i = 0; i < 7; i++)
	{
		for (uint8_t j = 0; j < 4; j++)
		{
			if (m_GM6020[j][i] != nullptr)
			{
				m_GM6020[j][i]->ControlUpdate();
			}

			if (m_M2006[j][i] != nullptr)
			{
				m_M2006[j][i]->ControlUpdate();
			}

			if (m_M3508[j][i] != nullptr)
			{
				m_M3508[j][i]->ControlUpdate();
			}
		}
	}

	mcan_tx_frame_t tx_buf;
	memset(&tx_buf, 0, sizeof(tx_buf));

	if (m_send_update_flag[0][0])
	{
		uint32_t put_index = 0;
		tx_buf.std_id = 0x1FF;
		tx_buf.dlc = 0x08;

		memcpy(tx_buf.data_8, m_send_buffer[0][0], 8);

		mcan_transmit_via_txfifo_nonblocking(HPM_MCAN0, &tx_buf, &put_index);
		m_send_update_flag[0][0] = false;
	}

	if (m_send_update_flag[0][1])
	{
		uint32_t put_index = 0;
		tx_buf.std_id = 0x200;
		tx_buf.dlc = 0x08;

		memcpy(tx_buf.data_8, m_send_buffer[0][1], 8);

		mcan_transmit_via_txfifo_nonblocking(HPM_MCAN0, &tx_buf, &put_index);
		m_send_update_flag[0][1] = false;
	}

	if (m_send_update_flag[0][2])
	{
		uint32_t put_index = 0;
		tx_buf.std_id = 0x2FF;
		tx_buf.dlc = 0x08;

		memcpy(tx_buf.data_8, m_send_buffer[0][2], 8);

		mcan_transmit_via_txfifo_nonblocking(HPM_MCAN0, &tx_buf, &put_index);
		m_send_update_flag[0][2] = false;
	}

	if (m_send_update_flag[1][0])
	{
		uint32_t put_index = 0;
		tx_buf.std_id = 0x1FF;
		tx_buf.dlc = 0x08;

		memcpy(tx_buf.data_8, m_send_buffer[1][0], 8);

		mcan_transmit_via_txfifo_nonblocking(HPM_MCAN1, &tx_buf, &put_index);
		m_send_update_flag[1][0] = false;
	}

	if (m_send_update_flag[1][1])
	{
		uint32_t put_index = 0;
		tx_buf.std_id = 0x200;
		tx_buf.dlc = 0x08;

		memcpy(tx_buf.data_8, m_send_buffer[1][1], 8);

		mcan_transmit_via_txfifo_nonblocking(HPM_MCAN1, &tx_buf, &put_index);
		m_send_update_flag[1][1] = false;
	}

	if (m_send_update_flag[1][2])
	{
		uint32_t put_index = 0;
		tx_buf.std_id = 0x2ff;
		tx_buf.dlc = 0x08;

		memcpy(tx_buf.data_8, m_send_buffer[1][2], 8);

		mcan_transmit_via_txfifo_nonblocking(HPM_MCAN1, &tx_buf, &put_index);
		m_send_update_flag[1][2] = false;
	}

	if (m_send_update_flag[2][0])
	{
		uint32_t put_index = 0;
		tx_buf.std_id = 0x1FF;
		tx_buf.dlc = 0x08;

		memcpy(tx_buf.data_8, m_send_buffer[2][0], 8);

		mcan_transmit_via_txfifo_nonblocking(HPM_MCAN2, &tx_buf, &put_index);
		m_send_update_flag[2][0] = false;
	}

	if (m_send_update_flag[2][1])
	{
		uint32_t put_index = 0;
		tx_buf.std_id = 0x200;
		tx_buf.dlc = 0x08;

		memcpy(tx_buf.data_8, m_send_buffer[2][1], 8);

		mcan_transmit_via_txfifo_nonblocking(HPM_MCAN2, &tx_buf, &put_index);
		m_send_update_flag[2][1] = false;
	}

	if (m_send_update_flag[2][2])
	{
		uint32_t put_index = 0;
		tx_buf.std_id = 0x2ff;
		tx_buf.dlc = 0x08;

		memcpy(tx_buf.data_8, m_send_buffer[2][2], 8);

		mcan_transmit_via_txfifo_nonblocking(HPM_MCAN1, &tx_buf, &put_index);
		m_send_update_flag[2][2] = false;
	}

	if (m_send_update_flag[3][0])
	{
		uint32_t put_index = 0;
		tx_buf.std_id = 0x1FF;
		tx_buf.dlc = 0x08;

		memcpy(tx_buf.data_8, m_send_buffer[3][0], 8);

		mcan_transmit_via_txfifo_nonblocking(HPM_MCAN3, &tx_buf, &put_index);
		m_send_update_flag[3][0] = false;
	}

	if (m_send_update_flag[3][1])
	{
		uint32_t put_index = 0;
		tx_buf.std_id = 0x200;
		tx_buf.dlc = 0x08;

		memcpy(tx_buf.data_8, m_send_buffer[3][1], 8);

		mcan_transmit_via_txfifo_nonblocking(HPM_MCAN3, &tx_buf, &put_index);
		m_send_update_flag[3][1] = false;
	}

	if (m_send_update_flag[3][2])
	{
		uint32_t put_index = 0;
		tx_buf.std_id = 0x2ff;
		tx_buf.dlc = 0x08;

		memcpy(tx_buf.data_8, m_send_buffer[3][2], 8);

		mcan_transmit_via_txfifo_nonblocking(HPM_MCAN3, &tx_buf, &put_index);
		m_send_update_flag[3][2] = false;
	}
}

void DJIMotorCANManager::Receive(uint8_t channel, mcan_rx_message_t *_rx_msg)
{
	uint16_t index = _rx_msg->std_id - 0x201;

	if (index > 11)
	{
		return;
	}

	memcpy(m_receive_buffer[channel][index], _rx_msg->data_8, 8);

	m_receive_update_flag[channel][index] = true;
}

void DJIMotorCANManager::Add2Buffer(MCAN_Type *_can, int32_t _canId, int16_t _current)
{
	int _i = 0;

	if (_can == HPM_MCAN0)
	{
		_i = 0;
	}
	else if (_can == HPM_MCAN1)
	{
		_i = 1;
	}
	else if (_can == HPM_MCAN2)
	{
		_i = 2;
	}
	else if (_can == HPM_MCAN3)
	{
		_i = 3;
	}

	int index = 0;

	if (_canId < 0x205 && _canId >= 0x201)
	{
		index = _canId - 0x201;
		m_send_buffer[_i][1][index * 2] = _current >> 8;
		m_send_buffer[_i][1][index * 2 + 1] = _current & 0x00FF;
		m_send_update_flag[_i][1] = true;
	}
	else if (_canId < 0x20C && _canId >= 0x205)
	{
		index = _canId - 0x205;
		m_send_buffer[_i][0][index * 2] = _current >> 8;
		m_send_buffer[_i][0][index * 2 + 1] = _current & 0x00FF;
		m_send_update_flag[_i][0] = true;
	}
}

void DJIMotorCANManager::RegisterHandler(uint8_t _can_channel, int _canId, GM6020 *_GM6020)
{
	m_GM6020[_can_channel][_canId - 0x205] = _GM6020;
	_GM6020->sensorFeedBack.SetBufferPtr(m_receive_buffer[_can_channel][_canId - 0x201]);
}

void DJIMotorCANManager::RegisterHandler(uint8_t _can_channel, int _canId, M2006 *_M2006)
{
	m_M2006[_can_channel][_canId - 0x201] = _M2006;
	_M2006->sensorFeedBack.SetBufferPtr(m_receive_buffer[_can_channel][_canId - 0x201]);
}

void DJIMotorCANManager::RegisterHandler(uint8_t _can_channel, int _canId, M3508 *_M3508)
{
	m_M3508[_can_channel][_canId - 0x201] = _M3508;
	_M3508->sensorFeedBack.SetBufferPtr(m_receive_buffer[_can_channel][_canId - 0x201]);
}
