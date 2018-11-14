#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <string>
#include <memory>
#include <array>
#include <stdint.h>
#include <cmath>


using namespace std;

int16_t accel_x;
int16_t accel_y;
int16_t accel_z;
int16_t gyro_x;
int16_t gyro_y;
int16_t gyro_z;
int16_t magax;
int16_t maggy;

//double accel_x;
//double accel_y;
//double accel_z;
//double gyro_x;
//double gyro_y;
//double gyro_z;
//double magax;
//double maggy;

double accel_xd,accel_yd,accel_zd,gyro_xd,gyro_yd,gyro_zd;
const char* TEMP_OUT_L  =" 0x05";
const char* TEMP_OUT_H  =" 0x06";

const char* STATUS_M   =" 0x07";

const char* D_OUT_X_L_M=" 0x08";
const char* D_OUT_X_H_M=" 0x09";
const char* D_OUT_Y_L_M=" 0x0A";
const char* D_OUT_Y_H_M=" 0x0B";
const char* D_OUT_Z_L_M=" 0x0C";
const char* D_OUT_Z_H_M=" 0x0D";    
const char* WHO_AM_I   =" 0x0F";

const char* INT_CTRL_M =" 0x12";
const char* INT_SRC_M  =" 0x13";
const char* INT_THS_L_M=" 0x14";
const char* INT_THS_H_M=" 0x15";

const char* OFFSET_X_L_M  =" 0x16";
const char* OFFSET_X_H_M  =" 0x17";
const char* OFFSET_Y_L_M  =" 0x18";
const char* OFFSET_Y_H_M  =" 0x19";
const char* OFFSET_Z_L_M  =" 0x1A";
const char* OFFSET_Z_H_M  =" 0x1B";

const char* REFERENCE_X   =" 0x1C";
const char* REFERENCE_Y   =" 0x1D";
const char* REFERENCE_Z   =" 0x1E";

const char* CTRL0   =" 0x1F";
const char* CTRL1   =" 0x20";
const char* CTRL2   =" 0x21";
const char* CTRL3   =" 0x22";
const char* CTRL4   =" 0x23";
const char* CTRL5   =" 0x24";
const char* CTRL6   =" 0x25";
const char* CTRL7   =" 0x26";

const char* STATUS_A   =" 0x27";
      
const char* OUT_X_L_A  =" 0x28";
const char* OUT_X_H_A  =" 0x29";
const char* OUT_Y_L_A  =" 0x2A";
const char* OUT_Y_H_A  =" 0x2B";
const char* OUT_Z_L_A  =" 0x2C";
const char* OUT_Z_H_A  =" 0x2D";

const char* FIFO_CTRL  =" 0x2E";
const char* FIFO_SRC   =" 0x2F";

const char* IG_CFG1    =" 0x30";
const char* IG_SRC1    =" 0x31";
const char* IG_THS1    =" 0x32";
const char* IG_DUR1    =" 0x33";
const char* IG_CFG2    =" 0x34";
const char* IG_SRC2    =" 0x35";
const char* IG_THS2    =" 0x36";
const char* IG_DUR2    =" 0x37";

const char* CLICK_CFG  =" 0x38";
const char* CLICK_SRC  =" 0x39";
const char* CLICK_THS  =" 0x3A";

const char* TIME_LIMIT    =" 0x3B";
const char* TIME_LATENCY  =" 0x3C";
const char* TIME_WINDOW   =" 0x3D";

const char*  Act_THS  =" 0x3E";
const char*  Act_DUR  =" 0x3F";
//my code ends here


const char* deviceADDR = " 0x1d";

const char* accADDR = " 0x1d";
const char* gyrADDR = " 0x6b";
const char* PWR_MGMT_1 = " 0x6B";
const char* ACCEL_X_OUT_H = " 0x29 ";
const char* ACCEL_X_OUT_L = " 0x28 ";
const char* ACCEL_Y_OUT_H = " 0x2B ";
const char* ACCEL_Y_OUT_L = " 0x2A ";
const char* ACCEL_Z_OUT_H = " 0x2D ";
const char* ACCEL_Z_OUT_L = " 0x2C ";
const char* GYRO_X_OUT_H = " 0x29 ";
const char* GYRO_X_OUT_L = " 0x28 ";
const char* GYRO_Y_OUT_H = " 0x2B ";
const char* GYRO_Y_OUT_L = " 0x2A ";
const char* GYRO_Z_OUT_H = " 0x2D ";
const char* GYRO_Z_OUT_L = " 0x2C ";

const char* cmdGet = "i2cget -y 1";
const char* cmdSet = "i2cset -y 1";

//exec function that runs bash commands in c++

string exec(char* cmd) {
	string data;
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
  strcat(cmd," 2>&1");
	stream = popen(cmd, "r");

	if (stream) {
		while (!feof(stream))
			if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
			pclose(stream);
		}
	return data;
}




//function that performs geti2c
string get(const char* reg1, const char* reg2){
	char str[100];
	string str2;

	strcpy(str, cmdGet);
	strcat(str, reg1);
	strcat(str, reg2);

	str2 = exec(str);
	return str2;
}

//function that performs seti2c
void set(const char* reg1, const char* reg2, int value){
	char str[100];
	string str2;

	strcpy(str, cmdSet);
	strcat(str, reg1);
	strcat(str, reg2);
	strcat(str, to_string(value).c_str());

	str2 = exec(str);
}

int main(){
	//set(deviceADDR, PWR_MGMT_1, 0);	//turn on the MPU6050
	while(true){
		accel_x = stoi(get(accADDR, ACCEL_X_OUT_H), nullptr, 16) << 8 | stoi(get(accADDR, ACCEL_X_OUT_L), nullptr, 16);
		accel_y = stoi(get(accADDR, ACCEL_Y_OUT_H), nullptr, 16) << 8 | stoi(get(accADDR, ACCEL_Y_OUT_L), nullptr, 16);
		accel_z = stoi(get(accADDR, ACCEL_Z_OUT_H), nullptr, 16) << 8 | stoi(get(accADDR, ACCEL_Z_OUT_L), nullptr, 16);
    
		accel_x = ~accel_x +1 ;
		accel_y = ~accel_y +1 ;
		accel_z = ~accel_z +1 ;
    
    magax = sqrt(accel_x*accel_x + accel_y*accel_y +accel_z*accel_z);

    accel_xd =(double)9.8* accel_x/magax;
    accel_yd =(double)9.8* accel_y/magax;
    accel_zd =(double)9.8* accel_z/magax;
     
		gyro_x = stoi(get(gyrADDR, GYRO_X_OUT_H), nullptr, 16) << 8 | stoi(get(gyrADDR, GYRO_X_OUT_L), nullptr, 16);
		gyro_y = stoi(get(gyrADDR, GYRO_Y_OUT_H), nullptr, 16) << 8 | stoi(get(gyrADDR, GYRO_Y_OUT_L), nullptr, 16);
		gyro_z = stoi(get(gyrADDR, GYRO_Z_OUT_H), nullptr, 16) << 8 | stoi(get(gyrADDR, GYRO_Z_OUT_L), nullptr, 16);
		gyro_x = ~gyro_x+1 ;
		gyro_y = ~gyro_y+1 ;
		gyro_z = ~gyro_z+1 ;
    maggy = sqrt(gyro_x*gyro_x + gyro_y*gyro_y +gyro_z*gyro_z);
    gyro_xd =(double)- (gyro_x)*245/32768;
    gyro_yd =(double)- (gyro_y)*245/32768;
    gyro_zd =(double)- (gyro_z)*245/32768;
		cout << "X-acc: " << accel_xd << " Y-acc: " << accel_yd << " Z-acc: " << accel_zd << endl;
		cout << "X-gyro: " << gyro_xd << " Y-gyro: " << gyro_yd << " Z-gyro: " << gyro_zd << endl;

	}
	return 0;
}
