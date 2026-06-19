#include "predictor.h"
#include "system_data.h"

#define NUM_FEATURES 6

static const float w[NUM_FEATURES] = 
{
	1.1395996518578853f,
	-0.2218490063078467f,
	0.0541507123667482f,
	-0.015495787648127436f,
	-0.10026138040716726f,
	-0.1040735814839828f
};

static const float b = 111.33985862078649f;

static float prediction = 0.0f;

static float Predict(float *x)
{
	float y = b;
	for(int i = 0 ; i < NUM_FEATURES;i++)
	{
		y += w[i] * x[i];
	}
	return y;
}

void Predictor_Init(void)
{
	prediction = 0.0f;
}

void Predictor_Update(void)
{
	float x[NUM_FEATURES];
	
	x[0] = gWeather.pm25;
	x[1] = gWeather.pm25_prev1;
	x[2] = gWeather.pm25_prev2;
	x[3] = gWeather.pm25_prev3;
	x[3] = gWeather.temperature;
	x[5] = gWeather.pressure;
	
	prediction = Predict(x);
	
	gWeather.ml_prediction = prediction;
	
}

float Predictor_GetValue(void)
{
	return prediction;
}