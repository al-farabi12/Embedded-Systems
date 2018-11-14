#include "sensorsample.h"

SensorSample::SensorSample(){
    this->q0 = 0.0f;
    this->q1 = 0.0f;
    this->q2 = 0.0f;
    this->q3 = 0.0f;
}

SensorSample::SensorSample(float q0, float q1, float q2, float q3)
{
    this->q0 = q0;
    this->q1 = q1;
    this->q2 = q2;
    this->q3 = q3;
}
