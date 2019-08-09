#ifndef __RM_PROTOCOL__
#define __RM_PROTOCOL__

#include <algorithm>
#include <stdint.h>

#pragma pack(push, 1)

#define HEADER_SOF 0xA5
#define REF_PROTOCOL_FRAME_MAX_SIZE 128

#define REF_PROTOCOL_HEADER_SIZE sizeof(frame_header_struct_t)
#define REF_PROTOCOL_CMD_SIZE 2
#define REF_PROTOCOL_CRC16_SIZE 2
#define REF_HEADER_CRC_LEN (REF_PROTOCOL_HEADER_SIZE + REF_PROTOCOL_CRC16_SIZE)
#define REF_HEADER_CRC_CMDID_LEN (REF_PROTOCOL_HEADER_SIZE + REF_PROTOCOL_CRC16_SIZE + sizeof(uint16_t))
#define REF_HEADER_CMDID_LEN (REF_PROTOCOL_HEADER_SIZE + sizeof(uint16_t))

//RM协议内置命令码
typedef enum
{
    GAME_STATUS_CMD_ID = 0x0001
} referee_data_cmd_id_tpye;

//RM协议帧头结构体
typedef struct
{
    uint8_t SOF: 8;
    uint16_t data_length: 16;
    uint8_t seq: 8;
    uint8_t CRC8: 8;
} frame_header_struct_t;

//RM协议反序列化步骤枚举
typedef enum
{
    STEP_HEADER_SOF = 0,
    STEP_LENGTH_LOW = 1,
    STEP_LENGTH_HIGH = 2,
    STEP_FRAME_SEQ = 3,
    STEP_HEADER_CRC8 = 4,
    STEP_DATA_CRC16 = 5,
} unpack_step_e;

//RM协议反序列化结构体
typedef struct
{
    frame_header_struct_t *p_header;
    uint16_t data_len;
    uint8_t protocol_packet[REF_PROTOCOL_FRAME_MAX_SIZE];
    unpack_step_e unpack_step;
    uint16_t index;
} unpack_data_t;

//RM夏令营比赛服务器信息包
typedef struct
{
    struct castle_energy
    {
        uint8_t energy[2];
    } castle_energy[7];

    struct region_occupy
    {
        uint8_t status : 2;     // 0 = no one, 1 = weak, 2 = strong
        uint8_t belong : 2;     // 0 = no one, 1 = red, 2 = blue
        uint8_t resv : 4;
    } region_occupy[9][7];

    uint8_t car_location[2];
    uint8_t round_step_move[2];
    uint8_t round_remain_tick;
    uint8_t round_remain_cnt : 7;
    uint8_t round_team : 1;
    int16_t realtime_score[2];
} summer_camp_info_t;

//Unpack Struct
typedef struct
{
    frame_header_struct_t header;
    uint16_t cmd_id;

    uint8_t cmd_data[REF_PROTOCOL_FRAME_MAX_SIZE];
} data_frame_unpack_struct;

typedef void (*unpack_callback) (uint8_t* data);

void RMProtocolUnpack(unpack_data_t *p_obj, uint8_t byte, unpack_callback callback);

#endif