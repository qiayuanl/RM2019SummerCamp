#include "hardware.h"

/////////////////////////Global HW Handle///////////////////////////////

RobotHardware *HW_Handle = NULL;

RobotHardware *Hardware()
{
    if (HW_Handle == NULL)
    {
        HW_Handle = new RobotHardware();
    }

    return HW_Handle;
}

void ReleaseHardware()
{
    if (HW_Handle == NULL)
    {
        return;
    }

    delete HW_Handle;
    HW_Handle = NULL;
}

//////////////////////////HW Update//////////////////////////////

void RobotHardware::update()
{
    CAN_Motor_Update();
}

///////////////////////////HW Implementation/////////////////////////////

void CAN0_ReceiveHandlerProxy(can_frame_t *frame, void *ptr)
{
    ((RobotHardware *)ptr)->CAN0_ReceiveFrame(frame);
}

void CAN1_ReceiveHandlerProxy(can_frame_t *frame, void *ptr)
{
    ((RobotHardware *)ptr)->CAN1_ReceiveFrame(frame);
}

RobotHardware::RobotHardware()
{
    //Initialize HW
    can0_adapter.reception_handler_data = (void *)this;
    can0_adapter.reception_handler = &CAN0_ReceiveHandlerProxy;

    can0_adapter.open(HW_CAN0_ID);

    can1_adapter.reception_handler_data = (void *)this;
    can1_adapter.reception_handler = &CAN1_ReceiveHandlerProxy;

    can1_adapter.open(HW_CAN1_ID);
}

RobotHardware::~RobotHardware()
{
    can0_adapter.close();
    can1_adapter.close();
}

void RobotHardware::Motor_UpdateOffset(moto_measure_t *ptr, can_frame_t *frame)
{
    ptr->last_angle = ptr->angle;
    ptr->angle = (uint16_t)(frame->data[0] << 8 | frame->data[1]);
    ptr->real_current = (int16_t)(frame->data[2] << 8 | frame->data[3]);
    ptr->speed_rpm = ptr->real_current;
    ptr->given_current = (int16_t)(frame->data[4] << 8 | frame->data[5]) / -5;
    ptr->hall = frame->data[6];

    if (ptr->angle - ptr->last_angle > 4096)
        ptr->round_cnt--;
    else if (ptr->angle - ptr->last_angle < -4096)
        ptr->round_cnt++;

    ptr->total_angle = ptr->round_cnt * 8192 + ptr->angle - ptr->offset_angle;
}

void RobotHardware::CAN0_ReceiveFrame(can_frame_t *frame)
{
    switch (frame->can_id)
    {
    case CAN_3510Moto1_ID:
    case CAN_3510Moto2_ID:
    case CAN_3510Moto3_ID:
    case CAN_3510Moto4_ID:
    case CAN_3510Moto5_ID:
    case CAN_3510Moto6_ID:
    case CAN_3510Moto7_ID:
    case CAN_3510Moto8_ID:
    {
        uint8_t idx = frame->can_id - CAN_3510Moto1_ID; //first 8 motors

        if (idx < HW_MOTOR_COUNT)
        {
            Motor_UpdateOffset(&motors[idx], frame);
            motors[idx].msg_cnt++;
        }

        break;
    }
    }
}

void RobotHardware::CAN1_ReceiveFrame(can_frame_t *frame)
{
    switch (frame->can_id)
    {
    case CAN_3510Moto1_ID:
    case CAN_3510Moto2_ID:
    case CAN_3510Moto3_ID:
    case CAN_3510Moto4_ID:
    case CAN_3510Moto5_ID:
    case CAN_3510Moto6_ID:
    case CAN_3510Moto7_ID:
    case CAN_3510Moto8_ID:
    {
        uint8_t idx = frame->can_id - CAN_3510Moto1_ID + 8; //last 8 motors

        if (idx < HW_MOTOR_COUNT)
        {
            Motor_UpdateOffset(&motors[idx], frame);
            motors[idx].msg_cnt++;
        }

        break;
    }
    }
}

void RobotHardware::CAN_Motor_Update()
{
    can_frame_t frame;

    //CAN0 Transmit Frame 1
    frame.can_id = HW_CAN_MOTOR_ID_1;
    frame.can_dlc = 8;

    for (int id = 0; id < std::min(4, HW_MOTOR_COUNT); id++)
    {
        int16_t power = (int16_t)motors[id].power;

        frame.data[2 * id] = (uint8_t)(power >> 8);
        frame.data[2 * id + 1] = (uint8_t)(power);
    }

    if (can0_adapter.is_open()) can0_adapter.transmit(&frame);

    //CAN0 Transmit Frame 2
    if (HW_MOTOR_COUNT > 4)
    {
        frame.can_id = HW_CAN_MOTOR_ID_2;
        frame.can_dlc = 8;

        for (int id = 4; id < std::min(8, HW_MOTOR_COUNT); id++)
        {
            int16_t power = (int16_t)motors[id].power;

            frame.data[2 * (id - 4)] = (uint8_t)(power >> 8);
            frame.data[2 * (id - 4) + 1] = (uint8_t)(power);
        }

        if (can0_adapter.is_open()) can0_adapter.transmit(&frame);
    }

    //CAN1 Transmit Frame 1
    if (HW_MOTOR_COUNT > 8)
    {
        frame.can_id = HW_CAN_MOTOR_ID_1;
        frame.can_dlc = 8;

        for (int id = 8; id < std::min(12, HW_MOTOR_COUNT); id++)
        {
            int16_t power = (int16_t)motors[id].power;

            frame.data[2 * (id - 8)] = (uint8_t)(power >> 8);
            frame.data[2 * (id - 8) + 1] = (uint8_t)(power);
        }

        if (can1_adapter.is_open()) can1_adapter.transmit(&frame);
    }

    //CAN1 Transmit Frame 2
    if (HW_MOTOR_COUNT > 12)
    {
        frame.can_id = HW_CAN_MOTOR_ID_2;
        frame.can_dlc = 8;

        for (int id = 12; id < std::min(16, HW_MOTOR_COUNT); id++)
        {
            int16_t power = (int16_t)motors[id].power;

            frame.data[2 * (id - 12)] = (uint8_t)(power >> 8);
            frame.data[2 * (id - 12) + 1] = (uint8_t)(power);
        }

        if (can1_adapter.is_open()) can1_adapter.transmit(&frame);
    }
}