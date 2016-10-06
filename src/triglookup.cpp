#include "triglookup.h"

 //TRIG LOOKUP 

 float TrigLookUp::sine[LOOKUP_SIZE];
 float TrigLookUp::cosine[LOOKUP_SIZE];
 
float lsin(int theta)
{
	theta = theta % LOOKUP_SIZE;
	return TrigLookUp::sine[theta];
}

float lcos(int theta)
{
	theta = theta % LOOKUP_SIZE;
	return TrigLookUp::cosine[theta];
}

void TrigLookUp::calculateTrigValues()
{
	for(int i = 0;i<LOOKUP_SIZE;i++)
	{
		TrigLookUp::sine[i]=sin(fmod(((float)i/(float)LOOKUP_SIZE)*PI2,PI2));
	}
	TrigLookUp::sine[LOOKUP_SIZE/2] = 0;

	for(int i = 0;i<LOOKUP_SIZE;i++)
	{
		TrigLookUp::cosine[i]=cos(fmod(((float)i/(float)LOOKUP_SIZE)*PI2,PI2));
	}
}
