#ifndef  __INV_MPU_DRIVE_H
#define __INV_MPU_DRIVE_H

#include "stm32f4xx.h"

s8 mpu6050_init(u8 dmp_enable);

s8 MPU_Get_Temperature(short *t);
s8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
s8 MPU_Get_Accelerometer(short *ax,short *ay,short *az);

static inline s8 run_self_test(void);
u8 mpu_dmp_get_data(short *gyro, short *accel,float *pitch,float *roll,float *yaw);


#endif
