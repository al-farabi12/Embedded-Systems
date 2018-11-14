#include<iostream>
#include<sstream>
#include<vector>
#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
//#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include<iomanip>
#include<math.h>
#include"MadgwickAHRS.h"
#include "SocketServer.h" 

using namespace std;
//using namespace exploringBB;

// Small macro to display value in hexadecimal with 2 places
#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)
#define DEVID       0x0F
//Settings for gyro
#define POWER_CTL1      0x20
#define POWER_CTL2      0x21
#define POWER_CTL4      0x23
#define LOW_ODR         0x39
//Settings for accel and mag
#define CTR1            0x20
#define CTR2            0x21
#define CTR5            0x24
#define CTR6            0x25
#define CTR7            0x26
//GYRO
#define GYROX0      0x28
#define GYROX1      0x29
#define GYROY0      0x2A
#define GYROY1      0x2B
#define GYROZ0      0x2C
#define GYROZ1      0x2D
//ACCEL
#define ACCELX0      0x28
#define ACCELX1      0x29
#define ACCELY0      0x2A
#define ACCELY1      0x2B
#define ACCELZ0      0x2C
#define ACCELZ1      0x2D
//MAG
#define MAGX0      0x08
#define MAGX1      0x09
#define MAGY0      0x0A
#define MAGY1      0x0B
#define MAGZ0      0x0C
#define MAGZ1      0x0D
#define BUFFER_SIZE 0x40
unsigned char dataBuffer[BUFFER_SIZE];

// Definitions
#define sampleFreq      100.0f          // sample frequency in Hz

#define betaDef         0.3f            // 2 * proportional gain



//---------------------------------------------------------------------------------------------------

// Variable definitions



volatile float beta = betaDef;                                                          // 2 * proportional gain (Kp)

volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;      // quaternion of sensor frame relative to auxiliary frame



//---------------------------------------------------------------------------------------------------

// Function declarations



float invSqrt(float x);
void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float hx, hy;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	if((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
		MadgwickAHRSupdateIMU(gx, gy, gz, ax, ay, az);
		return;
	}

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

		// Normalise magnetometer measurement
		recipNorm = invSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0mx = 2.0f * q0 * mx;
		_2q0my = 2.0f * q0 * my;
		_2q0mz = 2.0f * q0 * mz;
		_2q1mx = 2.0f * q1 * mx;
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_2q0q2 = 2.0f * q0 * q2;
		_2q2q3 = 2.0f * q2 * q3;
		q0q0 = q0 * q0;
		q0q1 = q0 * q1;
		q0q2 = q0 * q2;
		q0q3 = q0 * q3;
		q1q1 = q1 * q1;
		q1q2 = q1 * q2;
		q1q3 = q1 * q3;
		q2q2 = q2 * q2;
		q2q3 = q2 * q3;
		q3q3 = q3 * q3;

		// Reference direction of Earth's magnetic field
		hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
		hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
		_2bx = sqrt(hx * hx + hy * hy);
		_2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
		_4bx = 2.0f * _2bx;
		_4bz = 2.0f * _2bz;

		// Gradient decent algorithm corrective step
		s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
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

float invSqrt(float x) {

        float halfx = 0.5f * x;

        float y = x;

        long i = *(long*)&y;

        i = 0x5f3759df - (i>>1);

        y = *(float*)&i;

        y = y * (1.5f - (halfx * y * y));

        return y;

}


//====================================================================================================

// END OF CODE

//====================================================================================================

int writeRegister(int file, unsigned char address, char value){
   unsigned char buffer[2];
   buffer[0] = address;
   buffer[1] = value;
   if (write(file, buffer, 2)!=2){
      cout << "Failed write to the device" << endl;
      return 1;
   }
   return 0;
}

int readRegisters(int file, char reg){
   char x;

   write(file,&reg,1);
   read(file,&x,1);

   return x;
}

// short is 16-bits in size on the BBB
short combineValues(unsigned char msb, unsigned char lsb){
   //shift the msb right by 8 bits and OR with lsb
   return ((short)msb<<8)|(short)lsb;
}
int main(){
   int file1;
   int file2;
   cout << "Starting the process" << endl;
   if((file1=open("/dev/i2c-1", O_RDWR)) < 0){
      cout << "failed to open the bus" << endl;
      return 1;
   }
   if((file2=open("/dev/i2c-1", O_RDWR)) < 0){
      cout << "failed to open the bus" << endl;
      return 1;
   }

   if(ioctl(file1, I2C_SLAVE, 0x6b) < 0){
      cout << "Failed to connect to the sensor" << endl;
      return 1;
   }

   if(ioctl(file2, I2C_SLAVE, 0x1d) < 0){
      cout << "Failed to connect to the sensor" << endl;
      return 1;
   }

   writeRegister(file1, POWER_CTL1, 0xFF);
   writeRegister(file1, POWER_CTL2, 0x20);
   writeRegister(file1, POWER_CTL4, 0x00);
   writeRegister(file1, LOW_ODR, 0x01);
   writeRegister(file2, CTR1, 0x57);
   writeRegister(file2, CTR2, 0xC0);
   writeRegister(file2, CTR5, 0x64);
   writeRegister(file2, CTR6, 0x20);
   writeRegister(file2, CTR7, 0x00);


   cout << "The Device ID of Gyro is: 0x" << HEX(readRegisters(file1, DEVID)) << endl;
   cout << "The Device ID of Accel and Magn is: 0x" << HEX(readRegisters(file2, DEVID)) << endl;
   cout << dec << endl;

   // Now loop a display the x, y, z accelerometer for 60 seconds   int 
      cout << "Starting EBB Server Example" << endl;
      SocketServer server(5555);
        server.listen();

while(true){
      float gyrox = combineValues(readRegisters(file1,GYROX1), readRegisters(file1,GYROX0));
      float gyroy = combineValues(readRegisters(file1,GYROY1), readRegisters(file1,GYROY0));
      float gyroz = combineValues(readRegisters(file1,GYROZ1), readRegisters(file1,GYROZ0));
      float accelx = combineValues(readRegisters(file2,ACCELX1), readRegisters(file2,ACCELX0));
      float accely = combineValues(readRegisters(file2,ACCELY1), readRegisters(file2,ACCELY0));
      float accelz = combineValues(readRegisters(file2,ACCELZ1), readRegisters(file2,ACCELZ0));
      float magx = combineValues(readRegisters(file2,MAGX1), readRegisters(file2,MAGX0));
      float magy = combineValues(readRegisters(file2,MAGY1), readRegisters(file2,MAGY0));
      float magz = combineValues(readRegisters(file2,MAGZ1), readRegisters(file2,MAGZ0));
      //Use \r and flush to write the output on the same line
      gyrox = gyrox*0.00875*3.1415926/180;
      gyroy = gyroy*0.00875*3.1415926/180;
      gyroz = gyroz*0.00875*3.1415926/180;
    
      stringstream ss;
       MadgwickAHRSupdate(gyrox,gyroy,gyroz,accelx,accely,accelz,magx,magy,magz);
        ss<<"<sample><quaternions><q0>"<<q0<<"</q0>";
        ss<<"<q1>"<<q1<<"</q1>";
        ss<<"<q2>"<<q2<<"</q2>";
        ss<<"<q3>"<<q3<<"</q3></quaternions></sample>";
        cout<<ss.str()<<'\xd';
        cout.flush();

        server.send(ss.str());
//      cout << "End of EBB Server Example" << endl;
//      cout << "gyrox="<<gyrox<<" gyroy="<<gyroy<<" gyroz="<<gyroz<<"          accelx = "<<accelx<<" accely = "<<accely<<" accelz = "<<accelz<<"               magx = "<<magx<<" magy = "<<magy<<" magz = $
//      cout<<"q0 = "<<q0<<" q1 = "<<q1<<" q2 = "<<q2<<" q3 = "<<q3<< endl<<endl;
       //read the sensor again
   }
   close(file1);
   close(file2);
   return 0;
}


