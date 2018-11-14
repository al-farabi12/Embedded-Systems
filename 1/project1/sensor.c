/** Simple I2C example to read the first address of a device in C
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/i2c-dev.h>
//#include <unistd.h>
// Small macro to display value in hexadecimal with 2 places
#define L3GD20H		0x6b
#define LSM303D		0x1d
#define betaDef		0.1f
#define GYRO_X		0x29
#define GYRO_Y		0x2B
#define GYRO_Z		0x2D

#define ACC_X		0x29
#define ACC_Y		0x2B
#define ACC_Z		0x2D

#define BUFFER_SIZE 50
volatile float beta = betaDef;
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
#define sampleFreq	512.0f
int file;

float invSqrt(float x);

float invSqrt(float x) {
        float halfx = 0.5f * x;
        float y = x;
        long i = *(long*)&y;
        i = 0x5f3759df - (i>>1);
        y = *(float*)&i;
        y = y * (1.5f - (halfx * y * y));
        return y;
}


void selectDevice(int addr){
	if((file=open("/dev/i2c-1", O_RDWR)) < 0){
		perror("Failed to open the bus\n");
	}

	if(ioctl(file, I2C_SLAVE, addr) < 0){
		perror("Failed to connect to the sensor\n");
	}
}

void resetReadAddress(){
	char writeBuffer[1] = {0x00};
	if(write(file, writeBuffer, 1)!=1){
		perror("Failed to reset the read addresses\n");
	}
}

void readBlock(int *data, int addr){
	char buffer[BUFFER_SIZE];
	if (read(file, buffer, BUFFER_SIZE) != BUFFER_SIZE){
		perror("Failed to read the buffer\n");
	}
	data = (int16_t)buffer[addr];
}
void readGX(int *data){
	selectDevice(L3GD20H);
	resetReadAddress();
	readBlock(data, GYRO_X);
	close(file);
}
void readGY(int *data){
	selectDevice(L3GD20H);
	resetReadAddress();
	readBlock(data, GYRO_Y);
	close(file);
}
void readGZ(int *data){
	selectDevice(L3GD20H);
	resetReadAddress();
	readBlock(data, GYRO_Z);
	close(file);
}
void readAX(int *data){
	selectDevice(LSM303D);
	resetReadAddress();
	readBlock(data, ACC_X);
	close(file);
}
void readAY(int *data){
	selectDevice(LSM303D);
	resetReadAddress();
	readBlock(data, ACC_Y);
	close(file);
}
void readAZ(int *data){
	selectDevice(LSM303D);
	resetReadAddress();
	readBlock(data, ACC_Z);
	close(file);
}


void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az) {
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
	qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
	qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
	qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;   

		// Auxiliary variables to avoid repeated arithmetic
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_4q0 = 4.0f * q0;
		_4q1 = 4.0f * q1;
		_4q2 = 4.0f * q2;
		_8q1 = 8.0f * q1;
		_8q2 = 8.0f * q2;
		q0q0 = q0 * q0;
		q1q1 = q1 * q1;
		q2q2 = q2 * q2;
		q3q3 = q3 * q3;

		// Gradient decent algorithm corrective step
		s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
		s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
		s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
		s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	q0 += qDot1 * (1.0f / sampleFreq);
	q1 += qDot2 * (1.0f / sampleFreq);
	q2 += qDot3 * (1.0f / sampleFreq);
	q3 += qDot4 * (1.0f / sampleFreq);

	// Normalise quaternion
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}
int main(int argc, char *argv[]){
   int *p_gyrx;
   int *p_gyry;
   int *p_gyrz;
   int gyrx;
   int gyry;
   int gyrz;

   int *p_accx;
   int *p_accy;
   int *p_accz;
   int accx;
   int accy;
   int accz;

   p_gyrx = gyrx;
   p_gyry = gyry;
   p_gyrz = gyrz;

   p_accx = accx;
   p_accy = accy;
   p_accz = accz;
   while(1){
		readGX(p_gyrx);
		readGY(p_gyry);
		readGZ(p_gyrz);

		readAX(p_accx);
		readAY(p_accy);
		readAZ(p_accz);

		MadgwickAHRSupdateIMU((float)gyrx,(float)gyry,(float)gyrz,(float)accx,(float)accy,(float)accz);

		printf("Q0: %5f \t Q1: %5f \t Q2: %5f \t Q3: %5f \t \n", q0, q1, q2, q3);
	   	}
   }
