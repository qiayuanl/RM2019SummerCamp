#include "rm_protocol.h"
#include "crc/CRC8_CRC16.h"

//RM协议反序列化
void RMProtocolUnpack(unpack_data_t *p_obj, uint8_t byte, unpack_callback callback)
{
    uint8_t sof = HEADER_SOF;

    switch (p_obj->unpack_step)
    {
        //查找帧头
        case STEP_HEADER_SOF:
        {
            if (byte == sof)
            {
                p_obj->unpack_step = STEP_LENGTH_LOW;
                p_obj->protocol_packet[p_obj->index++] = byte;
            }
            else
            {
                p_obj->index = 0;
            }
        }
        break;

        //获取数据长度低字节
        case STEP_LENGTH_LOW:
        {
            p_obj->data_len = byte;
            p_obj->protocol_packet[p_obj->index++] = byte;
            p_obj->unpack_step = STEP_LENGTH_HIGH;
        }
        break;

        //获取数据长度高字节
        case STEP_LENGTH_HIGH:
        {
            p_obj->data_len |= (byte << 8);
            p_obj->protocol_packet[p_obj->index++] = byte;

            if (p_obj->data_len < (REF_PROTOCOL_FRAME_MAX_SIZE - REF_HEADER_CRC_CMDID_LEN))
            {
                p_obj->unpack_step = STEP_FRAME_SEQ;
            }
            else
            {
                p_obj->unpack_step = STEP_HEADER_SOF;
                p_obj->index = 0;
            }
        }
        break;

        //记录协议包序列号
        case STEP_FRAME_SEQ:
        {
            p_obj->protocol_packet[p_obj->index++] = byte;
            p_obj->unpack_step = STEP_HEADER_CRC8;
        }
        break;

        //校验帧头CRC8
        case STEP_HEADER_CRC8:
        {
            p_obj->protocol_packet[p_obj->index++] = byte;

            if (p_obj->index == REF_PROTOCOL_HEADER_SIZE)
            {
                if (Verify_CRC8_Check_Sum(p_obj->protocol_packet, REF_PROTOCOL_HEADER_SIZE))
                {
                    p_obj->unpack_step = STEP_DATA_CRC16;
                }
                else
                {
                    p_obj->unpack_step = STEP_HEADER_SOF;
                    p_obj->index = 0;
                }
            }
        }
        break;

        //校验整帧CRC16
        case STEP_DATA_CRC16:
        {
            if (p_obj->index < (REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
            {
                p_obj->protocol_packet[p_obj->index++] = byte;
            }
            if (p_obj->index >= (REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
            {
                p_obj->unpack_step = STEP_HEADER_SOF;
                p_obj->index = 0;

                if (Verify_CRC16_Check_Sum(p_obj->protocol_packet, REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
                {
                    //成功解到一个正确的信息包
                    callback(p_obj->protocol_packet);
                }
            }
        }
        break;

        //解包失败重新寻找帧头
        default:
        {
            p_obj->unpack_step = STEP_HEADER_SOF;
            p_obj->index = 0;
        }
        break;
    }
}