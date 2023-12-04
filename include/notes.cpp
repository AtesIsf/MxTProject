#include <cmath>
#include <cstddef>
#include "notes.hpp"

// f' = f * 2^(N/12)

int * ScaleFormula(bool is_maj)
{
	// Maj: wwhwwwh
	// Min: whwwhww
	if (is_maj)
		return new int[] {2, 2, 1, 2, 2, 2, 1};
	return new int[] {2, 1, 2, 2, 1, 2, 2};
}

float * GetScale(float start, bool is_maj)
{
	int *formula = ScaleFormula(is_maj);	
	// calculate lowest root (2 octaves below)
	float root = start / 4;	
	
	float *scale = new float[28];

	int counter = 0;
	int total_steps = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			total_steps += formula[j];
			scale[counter] = root * std::pow(2, total_steps/12);
		}
	}

	delete formula;

	return scale;
}

