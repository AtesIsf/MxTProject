#include <cmath>
#include <algorithm>
#include <random>
#include "def.hpp"

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
			scale[counter] = root * std::pow(2, ((float)total_steps)/12);
			counter++;
		}
	}

	delete formula;

	return scale;
}

void CurrKeyStr(data_t *d)
{
	// Note
	if (d->note == C)
	{
		d->key_str[0] = 'C';
		d->key_str[1] = ' ';
	}
	else if (d->note == CS)
	{
		d->key_str[0] = 'C';
		d->key_str[1] = '#';
	}
	else if (d->note == D)
	{
		d->key_str[0] = 'D';
		d->key_str[1] = ' ';
	}	
	else if (d->note == DS)
	{
		d->key_str[0] = 'D';
		d->key_str[1] = '#';
	}	
	else if (d->note == E)
	{
		d->key_str[0] = 'E';
		d->key_str[1] = ' ';
	}	
	else if (d->note == F)
	{
		d->key_str[0] = 'F';
		d->key_str[1] = ' ';
	}	
	else if (d->note == FS)
	{
		d->key_str[0] = 'F';
		d->key_str[1] = '#';
	}	
	else if (d->note == G)
	{
		d->key_str[0] = 'G';
		d->key_str[1] = ' ';
	}	
	else if (d->note == GS)
	{
		d->key_str[0] = 'G';
		d->key_str[1] = '#';
	}	
	else if (d->note == A)
	{
		d->key_str[0] = 'A';
		d->key_str[1] = ' ';
	}	
	else if (d->note == AS)
	{
		d->key_str[0] = 'A';
		d->key_str[1] = '#';
	}	
	else if (d->note == B)
	{
		d->key_str[0] = 'B';
		d->key_str[1] = ' ';
	}	

	// Maj/Min
	if (d->is_maj)
		d->key_str[2] = 'M';
	else d->key_str[2] = 'm';
}

void NoteCharMatchup(data_t *d)
{
	// Indexes -> a to z, last 2 are space and full stop
	float *scale = GetScale(d->note, d->is_maj);
	float *temp = d->char_freq;
	delete temp;

	d->char_freq = scale;

	auto rd = std::random_device {}; 
	auto rng = std::default_random_engine { rd() };
	std::random_shuffle(&(d->char_freq[0]), &(d->char_freq[27]));
}

int CharIndex(data_t *d, char c)
{
	// 65-90 97-122 space: 32 full stop: 46
	int index = -1;

	// space
	if (c == 32)
		index = 26;

	// full stop
	else if (c == 46)
		index = 27;
	
	if (c >= 97 && c <= 122)
		index = c-97;
	
	if (c >= 65 && c <= 90)
		index = c-65;
	
	return index;
}

