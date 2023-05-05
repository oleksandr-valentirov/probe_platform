#include "imu.h"


static void (*sensor_main)(int*, int*, int*, int*, int*, int*) = NULL;
int x, y, z;

static int *const ca_x = IMU_AX,
           *const ca_y = IMU_AY,
           *const ca_z = IMU_AZ,
           *const cg_x = IMU_GX,
           *const cg_y = IMU_GY,
           *const cg_z = IMU_GZ;

void IMU_SetCalibAX(int x) {*ca_x = x;}
void IMU_SetCalibAY(int y) {*ca_y = y;}
void IMU_SetCalibAZ(int z) {*ca_z = z;}

void IMU_SetCalibGX(int x) {*cg_x = x;}
void IMU_SetCalibGY(int y) {*cg_y = y;}
void IMU_SetCalibGZ(int z) {*cg_z = z;}


static void IMU_Main(void);


void* IMU_Init(void)
{
    sensor_main = (void(*)(int*, int*, int*, int*, int*, int*))IMU_SensorInit();
    
    if(sensor_main == NULL)
    {
        return NULL;
    }
    
    if(1)
    {   
        /* 
        if position is stable, calc start angles from accelerometr vectors
        θ = arctan(ax / sqrt(ay^2 + az^2))
        θ - angle of inclination along the X-axis in radians,
        ax - measured acceleration along the X-axis in meters per second squared,
        ay - measured acceleration along the Y-axis in meters per second squared,
        az - measured acceleration along the Z-axis in meters per second squared.
        */
        int ax, ay, az;
        IMU_poll_accel(&ax, &ay, &az);
    }
    
    return (void*)IMU_Main;
}

static void IMU_Main(void)
{
    int ax, ay, az, gx, gy, gz;
    sensor_main(&ax, &ay, &az, &gx, &gy, &gz);
    
    // perform calculations and store to global variables
}