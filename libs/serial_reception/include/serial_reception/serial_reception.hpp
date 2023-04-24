#pragma once

#include <assert.h>
#include <algorithm>
#include <stdint.h>
#include <cstring>

struct SerialReceptionConfig
{
    using get_counter_cb = uint32_t(*)(void);
    using dma_read_cb = void(*)(uint8_t*, uint32_t);
    get_counter_cb get_dma_counter;
    dma_read_cb dma_read;
    bool use_idle;
};

template<int INCOMING_DATA_SIZE>
class SerialReception
{
    public:
        using incoming_data_callback = bool(*)(const uint8_t*, uint32_t);

        SerialReception(const SerialReceptionConfig& config) :
            m_serial_config(config) { }

       void init()
       {
            assert(m_serial_config.get_dma_counter != nullptr);
            assert(m_serial_config.dma_read != nullptr);

            configure_read_from_serial();
       }

        void idle_detection()
        {
            m_idle_detected = true;
        }

        void error()
        {
            configure_read_from_serial();
        }

        // This is the case where m_incoming_data_callback is not used, and instead we read the serial
        // periodically
        uint32_t read(uint8_t* rx_data, uint32_t max_rx_size)
        {
            if (m_serial_config.use_idle && m_idle_detected == false)
            {
                return 0;
            }

            m_idle_detected = false;
            uint32_t total_byte_read = 0;
            uint32_t dma_write_position = INCOMING_DATA_SIZE - m_serial_config.get_dma_counter();

            if (m_last_dma_write_pos != dma_write_position)
            {
                uint32_t read_size = 0;
                if (dma_write_position > m_last_dma_write_pos)
                {
                    // We are in "linear" mode - Process data directly by subtracting "pointers"
                    read_size = std::min(dma_write_position - m_last_dma_write_pos, max_rx_size);
                    std::memcpy(rx_data, m_incoming_data + m_last_dma_write_pos, read_size);
                    total_byte_read += read_size;
                    m_last_dma_write_pos = m_last_dma_write_pos + read_size;
                }
                else
                {
                    // We are in "overflow" mode - First process data to the end of buffer
                    read_size = std::min(INCOMING_DATA_SIZE - m_last_dma_write_pos, max_rx_size);
                    std::memcpy(rx_data, m_incoming_data + m_last_dma_write_pos, read_size);
                    total_byte_read += read_size;

                    // Need to keep reading?
                    if (dma_write_position > 0)
                    {
                        read_size = std::min(dma_write_position, max_rx_size - total_byte_read);
                        std::memcpy(rx_data + total_byte_read, m_incoming_data, read_size);
                        total_byte_read += read_size;
                        m_last_dma_write_pos = read_size;
                    }
                    else
                    {
                        m_last_dma_write_pos = m_last_dma_write_pos + read_size;
                    }
                }
            }

            return total_byte_read;
        }

    private:
        const SerialReceptionConfig& m_serial_config;
        uint8_t m_incoming_data[INCOMING_DATA_SIZE];
        uint32_t m_last_dma_write_pos;
        bool m_idle_detected;

        void configure_read_from_serial()
        {
            m_last_dma_write_pos = 0;
            m_idle_detected = false;
            m_serial_config.dma_read(m_incoming_data, INCOMING_DATA_SIZE);
        }
};
