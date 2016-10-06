#pragma once

#define LOOKUP_SIZE 5000
#define PI2 2*3.141592f
#include <math.h>
#include <iostream>

	class TrigLookUp
	{
	public:
		static float sine[LOOKUP_SIZE];
		static float cosine[LOOKUP_SIZE];
		static void calculateTrigValues();
	};

    float lsin(int theta);
	float lcos(int theta);
