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

void ReceiveHandlerProxy(can_frame_t *frame, void *ptr)
{
    ((RobotHardware *)ptr)->CAN_ReceiveFrame(frame);
}

RobotHardware::RobotHardware()
{
    //Initialize HW
    adapter.reception_handler_data = (void *)this;
    adapter.reception_handler = &ReceiveHandlerProxy;

    adapter.open(HW_CAN_ID);
}

RobotHardware::~RobotHardware()
{
    adapter.close();
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

void RobotHardware::Motor_UpdateTotalAngle(moto_measure_t *p)
{
    int res1, res2, delta;
    if (p->angle < p->last_angle)
    {
        res1 = p->angle + 8192 - p->last_angle;
        res2 = p->angle - p->last_angle;
    }
    else
    {
        res1 = p->angle - 8192 - p->last_angle;
        res2 = p->angle - p->last_angle;
    }

    if (abs(res1) < abs(res2))
        delta = res1;
    else
        delta = res2;

    p->total_angle += delta;
    p->last_angle = p->angle;
}

void RobotHardware::CAN_ReceiveFrame(can_frame_t *frame)
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
        uint8_t idx = frame->can_id - CAN_3510Moto1_ID;

        if (idx < HW_MOTOR_COUNT)
        {
            Motor_UpdateOffset(&motors[idx], frame);
            Motor_UpdateTotalAngle(&motors[idx]);

            motors[idx].msg_cnt++;
        }

        break;
    }

    case SINGLE_GYRO_ID:
    {
        gyro.angle = 0.001f * ((int32_t)((frame->data[0] << 24) | (frame->data[1] << 16) | (frame->data[2] << 8) | (frame->data[3])));
        gyro.gyro = 0.001f * ((int32_t)((frame->data[4] << 24) | (frame->data[5] << 16) | (frame->data[6] << 8) | (frame->data[7])));

        break;
    }
    }
}

void RobotHardware::CAN_Motor_Update()
{
    if (!adapter.is_open())
        return;

    can_frame_t frame;

    //Transmit Frame 1
    frame.can_id = HW_CAN_MOTOR_ID_1;
    frame.can_dlc = 8;

    for (int id = 0; id < std::min(HW_MOTOR_COUNT, 4); id++)
    {
        int16_t power = (int16_t)motors[id].power;

        frame.data[2 * id] = (uint8_t)(power >> 8);
        frame.data[2 * id + 1] = (uint8_t)(power);
    }

    adapter.transmit(&frame);

    if (HW_MOTOR_COUNT > 4)
    {
        //Transmit Frame 2
        frame.can_id = HW_CAN_MOTOR_ID_2;
        frame.can_dlc = 8;

        for (int id = 4; id < HW_MOTOR_COUNT; id++)
        {
            int16_t power = (int16_t)motors[id].power;

            frame.data[2 * (id - 4)] = (uint8_t)(power >> 8);
            frame.data[2 * (id - 4) + 1] = (uint8_t)(power);
        }

        adapter.transmit(&frame);
    }
}