
#include "Kalman.h"

Kalman::Kalman()
{
	QPos = 0.001f;
	QVel = 0.003f;
	R = 0.03f;

	pos = 0.0f;  // Reset the pos
	vel = 0.0f;  // Reset speed
	bias = 0.0f; // Reset bias of velocity

	P[0][0] = 0.0f; // Since we assume that the speed is 0 and we know the starting pos (use setpos), the error covariance matrix is set like so
	P[0][1] = 0.0f;
	P[1][0] = 0.0f;
	P[1][1] = 0.0f;
};

// The pos should be in degrees and the rate should be in degrees per second and the delta time in seconds
float Kalman::update(float newpos, float newVel, float dt)
{

	// Discrete Kalman filter time update equations - Time Update ("Predict")
	// Update xhat - Project the state ahead
	/* Step 1 */
	vel = newVel - bias;
	pos += dt * vel;

	// Update estimation error covariance - Project the error covariance ahead
	/* Step 2 */
	P[0][0] += dt * (dt * P[1][1] - P[0][1] - P[1][0] + QPos);
	P[0][1] -= dt * P[1][1];
	P[1][0] -= dt * P[1][1];
	P[1][1] += QVel * dt;

	// Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
	// Calculate Kalman gain - Compute the Kalman gain
	/* Step 4 */
	float S = P[0][0] + R; // Estimate error
	/* Step 5 */
	float K[2]; // Kalman gain - This is a 2x1 vector
	K[0] = P[0][0] / S;
	K[1] = P[1][0] / S;

	// Calculate pos and velocity - Update estimate with measurement zk (newpos)
	/* Step 3 */
	float y = newpos - pos; // pos difference
	/* Step 6 */
	pos += K[0] * y;
	vel += K[1] * y;

	// Calculate estimation error covariance - Update the error covariance
	/* Step 7 */
	float P00_temp = P[0][0];
	float P01_temp = P[0][1];

	P[0][0] -= K[0] * P00_temp;
	P[0][1] -= K[0] * P01_temp;
	P[1][0] -= K[1] * P00_temp;
	P[1][1] -= K[1] * P01_temp;

	return pos;
};

void Kalman::setpos(float pos) { this->pos = pos; }; // Used to set pos, this should be set as the starting pos
float Kalman::getPos() { return this->pos; };
float Kalman::getVel() { return this->vel; };
/* These are used to tune the Kalman filter */
void Kalman::setQpos(float QPos) { this->QPos = QPos; };
void Kalman::setQVel(float QVel) { this->QVel = QVel; };
void Kalman::setR(float R) { this->R = R; };

float Kalman::getQPos() { return this->QPos; };
float Kalman::getQVel() { return this->QVel; };
float Kalman::getR() { return this->R; };
