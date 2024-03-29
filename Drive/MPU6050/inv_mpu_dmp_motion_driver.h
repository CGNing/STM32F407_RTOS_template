/*
 $License:
    Copyright (C) 2011-2012 InvenSense Corporation, All Rights Reserved.
    See included License.txt for License information.
 $
 */
/**
 *  @addtogroup  DRIVERS Sensor Driver Layer
 *  @brief       Hardware drivers to communicate with sensors via I2C.
 *
 *  @{
 *      @file       inv_mpu_dmp_motion_driver.h
 *      @brief      DMP image and interface functions.
 *      @details    All functions are preceded by the dmp_ prefix to
 *                  differentiate among MPL and general driver function calls.
 */
#ifndef _INV_MPU_DMP_MOTION_DRIVER_H_
#define _INV_MPU_DMP_MOTION_DRIVER_H_

#define TAP_X               (0x01)
#define TAP_Y               (0x02)
#define TAP_Z               (0x04)
#define TAP_XYZ             (0x07)

#define TAP_X_UP            (0x01)
#define TAP_X_DOWN          (0x02)
#define TAP_Y_UP            (0x03)
#define TAP_Y_DOWN          (0x04)
#define TAP_Z_UP            (0x05)
#define TAP_Z_DOWN          (0x06)

#define ANDROID_ORIENT_PORTRAIT             (0x00)
#define ANDROID_ORIENT_LANDSCAPE            (0x01)
#define ANDROID_ORIENT_REVERSE_PORTRAIT     (0x02)
#define ANDROID_ORIENT_REVERSE_LANDSCAPE    (0x03)

#define DMP_INT_GESTURE     (0x01)
#define DMP_INT_CONTINUOUS  (0x02)

#define DMP_FEATURE_TAP             (0x001)//"轻敲"手势特性
#define DMP_FEATURE_ANDROID_ORIENT  (0x002)//此功能是兼容谷歌Motion Driver设备显示方向的实现,此功能包括一个状态机,它计算显示方向
#define DMP_FEATURE_LP_QUAT         (0x004)//DMP以200Hz的频率根据陀螺仪的数据计算三轴四元数,低功耗
#define DMP_FEATURE_PEDOMETER       (0x008)//计步特性
#define DMP_FEATURE_6X_LP_QUAT      (0x010)//DMP以200Hz的频率根据陀螺仪和加速度计的数据融合计算出四元数并输出,低功耗
#define DMP_FEATURE_GYRO_CAL        (0x020)//此特性将在每次设备静止超过8秒时,校准陀螺仪零偏
#define DMP_FEATURE_SEND_RAW_ACCEL  (0x040)//将加速计的raw轴(偏航)的数据放入FIFO,此数据基于芯片坐标系
#define DMP_FEATURE_SEND_RAW_GYRO   (0x080)//将陀螺仪的raw轴(偏航)的数据放入FIFO,此数据基于芯片坐标系
#define DMP_FEATURE_SEND_CAL_GYRO   (0x100)//将陀螺仪校准后的数据放入FIFO,不可与DMP_FEATURE_SEND_RAW_GYRO结合使用。输出不基于芯片框架,而是基于设备框架或者体框架

#define INV_WXYZ_QUAT       (0x100)

/* Set up functions. */
int dmp_load_motion_driver_firmware(void);//a.推送DMP映像到MPU内存
int dmp_set_fifo_rate(unsigned short rate);//e.数据速率
int dmp_get_fifo_rate(unsigned short *rate);
int dmp_enable_feature(unsigned short mask);//d.使能DMP特性
int dmp_get_enabled_features(unsigned short *mask);
int dmp_set_interrupt_mode(unsigned char mode);
int dmp_set_orientation(unsigned short orient);//b.推送陀螺仪和加速度计的方向矩阵到DMP
int dmp_set_gyro_bias(long *bias);
int dmp_set_accel_bias(long *bias);

/* Tap functions. */
int dmp_register_tap_cb(void (*func)(unsigned char, unsigned char));
int dmp_set_tap_thresh(unsigned char axis, unsigned short thresh);
int dmp_set_tap_axes(unsigned char axis);
int dmp_set_tap_count(unsigned char min_taps);
int dmp_set_tap_time(unsigned short time);
int dmp_set_tap_time_multi(unsigned short time);
int dmp_set_shake_reject_thresh(long sf, unsigned short thresh);
int dmp_set_shake_reject_time(unsigned short time);
int dmp_set_shake_reject_timeout(unsigned short time);

/* Android orientation functions. */
int dmp_register_android_orient_cb(void (*func)(unsigned char));//c.DMP回调函数

/* LP quaternion functions. */
int dmp_enable_lp_quat(unsigned char enable);
int dmp_enable_6x_lp_quat(unsigned char enable);

/* Pedometer functions. */
int dmp_get_pedometer_step_count(unsigned long *count);
int dmp_set_pedometer_step_count(unsigned long count);
int dmp_get_pedometer_walk_time(unsigned long *time);
int dmp_set_pedometer_walk_time(unsigned long time);

/* DMP gyro calibration functions. */
int dmp_enable_gyro_cal(unsigned char enable);

/* Read function. This function should be called whenever the MPU interrupt is
 * detected.
 */
int dmp_read_fifo(short *gyro, short *accel, long *quat,
    unsigned long *timestamp, short *sensors, unsigned char *more);

#endif  /* #ifndef _INV_MPU_DMP_MOTION_DRIVER_H_ */

