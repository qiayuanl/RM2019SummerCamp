#ifndef _Kalman_h_
#define _Kalman_h_

class Kalman
{
public:
	Kalman();

	float update(float newPos, float newVel, float dt);

	void setpos(float pos); // Used to set pos, this should be set as the starting pos
	float getPos();
	float getVel();

	/* These are used to tune the Kalman filter */
	void setQpos(float QPos);
	/**
     * setQVel(float QVel)
     * Default value (0.003f) is in Kalman.cpp.
     * Raise this to follow input more closely,
     * lower this to smooth result of kalman filter.
     */
	void setQVel(float QVel);
	void setR(float R);

	float getQPos();
	float getQVel();
	float getR();

private:
	/* Kalman filter variables */
	float QPos; // Process noise variance for the position
	float QVel; // Process noise variance for the velocity
	float R;	// Measurement noise variance - this is actually the variance of the measurement noise

	float pos;  // The pos calculated by the Kalman filter - part of the 2x1 state vector
	float vel;  // The gyro velocity calculated by the Kalman filter - part of the 2x1 state vector
	float bias; //The bias of velocity

	float P[2][2]; // Error covariance 2x2 matrix
};

#endif
