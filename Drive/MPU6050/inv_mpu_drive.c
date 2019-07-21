#include <math.h>

#include "i2c_soft.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "inv_mpu_drive.h"

#define DEFAULT_MPU_HZ  (100)

#define q30  1073741824.0f

/* The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from thei
 * driver(s).
 * TODO: The following matrices refer to the configuration on an internal test
 * board at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */
static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};

/* These next two functions converts the orientation matrix (see
 * gyro_orientation) to a scalar representation for use by the DMP.
 * NOTE: These functions are borrowed from Invensense's MPL.
 */
																					 
struct __mpu_offset{
	float pitch,roll,yaw;
	short gyro[3],accel[3];
};

struct __mpu_offset mpu_offset={
	.pitch=0,
	.roll=0,
	.yaw=0,
	.gyro={0,0,0},
	.accel={0,0,0}
};

static inline unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

static inline unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}


s8 mpu6050_init(u8 dmp_enable){
	unsigned char accel_fsr;
	unsigned short gyro_rate, gyro_fsr;
	unsigned short dmp_features;

	I2c_Soft_Init();

	if(mpu_init(0))
		return -1;

	/* If you're not using an MPU9150 AND you're not using DMP features, this
		 * function will place all slaves on the primary bus.*/
	//mpu_set_bypass(1);

	/* Get/set hardware configuration. Start gyro. */
	/* Wake up all sensors. */
	mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
	/* Push both gyro and accel data into the FIFO. */
	mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
	mpu_set_sample_rate(DEFAULT_MPU_HZ);
	/* Read back configuration in case it was set improperly. */
	mpu_get_sample_rate(&gyro_rate);
	mpu_get_gyro_fsr(&gyro_fsr);
	mpu_get_accel_fsr(&accel_fsr);
	
	if(dmp_enable){

		/* To initialize the DMP:
			 * 1. Call dmp_load_motion_driver_firmware(). This pushes the DMP image in
			 *    inv_mpu_dmp_motion_driver.h into the MPU memory.
			 * 2. Push the gyro and accel orientation matrix to the DMP.
			 * 3. Register gesture callbacks. Don't worry, these callbacks won't be
			 *    executed unless the corresponding feature is enabled.
			 * 4. Call dmp_enable_feature(mask) to enable different features.
			 * 5. Call dmp_set_fifo_rate(freq) to select a DMP output rate.
			 * 6. Call any feature-specific control functions.
			 *
			 * To enable the DMP, just call mpu_set_dmp_state(1). This function can
			 * be called repeatedly to enable and disable the DMP at runtime.
			 *
			 * The following is a short summary of the features supported in the DMP
			 * image provided in inv_mpu_dmp_motion_driver.c:
			 * DMP_FEATURE_LP_QUAT: Generate a gyro-only quaternion on the DMP at
			 * 200Hz. Integrating the gyro data at higher rates reduces numerical
			 * errors (compared to integration on the MCU at a lower sampling rate).
			 * DMP_FEATURE_6X_LP_QUAT: Generate a gyro/accel quaternion on the DMP at
			 * 200Hz. Cannot be used in combination with DMP_FEATURE_LP_QUAT.
			 * DMP_FEATURE_TAP: Detect taps along the X, Y, and Z axes.
			 * DMP_FEATURE_ANDROID_ORIENT: Google's screen rotation algorithm. Triggers
			 * an event at the four orientations where the screen should rotate.
			 * DMP_FEATURE_GYRO_CAL: Calibrates the gyro data after eight seconds of
			 * no motion.
			 * DMP_FEATURE_SEND_RAW_ACCEL: Add raw accelerometer data to the FIFO.
			 * DMP_FEATURE_SEND_RAW_GYRO: Add raw gyro data to the FIFO.
			 * DMP_FEATURE_SEND_CAL_GYRO: Add calibrated gyro data to the FIFO. Cannot
			 * be used in combination with DMP_FEATURE_SEND_RAW_GYRO.
			 */
			dmp_load_motion_driver_firmware();
			dmp_set_orientation(
					inv_orientation_matrix_to_scalar(gyro_orientation));
	//		dmp_register_tap_cb(tap_cb);
	//		dmp_register_android_orient_cb(android_orient_cb);
			dmp_features = DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
					DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
					DMP_FEATURE_GYRO_CAL;
			dmp_enable_feature(dmp_features);
			dmp_set_fifo_rate(DEFAULT_MPU_HZ);
			run_self_test();//自我检测并设置机械中值
			mpu_set_dmp_state(1);
		
	}
		return 0;
}

static inline s8 run_self_test(void)
{
    int result;
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if ((result&0x3) == 0x3) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
			
				return 0;
    }

    /* Report results. */
    return -1;
}

s8 MPU_Get_Temperature(short *t){
	long buf; 
	
	if(mpu_get_temperature(&buf,0))
		return -1;

  *t=(short)buf/655.36;
	return 0;
}

s8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz){
	short buf[3];  
	
	if(mpu_get_gyro_reg(buf,0))	
		return -1;
	
		*gx=buf[0];  
		*gy=buf[1];  
		*gz=buf[2];
    return 0;
}

s8 MPU_Get_Accelerometer(short *ax,short *ay,short *az){
	short buf[3];
	
	if(mpu_get_accel_reg(buf,0))	
		return -1;
	
		*ax=buf[0];  
		*ay=buf[1];  
		*az=buf[2];
    return 0;
}

//得到dmp处理后的数据(注意,本函数需要比较多堆栈,局部变量有点多)
//pitch:俯仰角 精度:0.1°   范围:-90.0° <---> +90.0°
//roll:横滚角  精度:0.1°   范围:-180.0°<---> +180.0°
//yaw:航向角   精度:0.1°   范围:-180.0°<---> +180.0°
//返回值:0,正常
//    其他,失败
u8 mpu_dmp_get_data(short *gyro, short *accel,float *pitch,float *roll,float *yaw)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	unsigned long sensor_timestamp;
	short  sensors;
	unsigned char more;
	long quat[4]; 
	if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more))return 1;	 
	/* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
	 * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
	**/
	/*if (sensors & INV_XYZ_GYRO )
	send_packet(PACKET_TYPE_GYRO, gyro);
	if (sensors & INV_XYZ_ACCEL)
	send_packet(PACKET_TYPE_ACCEL, accel); */
	/* Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
	 * The orientation is set by the scalar passed to dmp_set_orientation during initialization. 
	**/
	if(sensors&INV_WXYZ_QUAT) 
	{
		q0 = quat[0] / q30;	//q30格式转换为浮点数
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30; 
		//计算得到俯仰角/横滚角/航向角
		*pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	// pitch
		*roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	// roll
		*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	}else return 2;
	return 0;
}
