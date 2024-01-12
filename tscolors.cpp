#include <stdio.h>
#include "include/tscolors.hpp"
#include "include/tsutils.hpp"

void rgb2hsl(rgb_color *rgb, hsl_color *hsl) 
{
	float r = (rgb->r / 255.0f);
	float g = (rgb->g / 255.0f);
	float b = (rgb->b / 255.0f);

	float Min = min(min(r, g), b);
	float Max = max(max(r, g), b);
	float delta = Max - Min;

	hsl->l = (Max + Min) / 2;   // L

	if (delta == 0)
	{
		hsl->h = 0;             // H
		hsl->s = 0.0f;          // S
	}
	else
	{
		hsl->s = (hsl->l <= 0.5) ? // S 
            (delta / (Max + Min)) : 
            (delta / (2 - Max - Min));

		float hue;

		if (r == Max)
		{
			hue = ((g - b) / 6) / delta;
		}
		else if (g == Max)
		{
			hue = (1.0f / 3) + ((b - r) / 6) / delta;
		}
		else
		{
			hue = (2.0f / 3) + ((r - g) / 6) / delta;
		}

		if (hue < 0)
			hue += 1;
		if (hue > 1)
			hue -= 1;

		hsl->h = (int)(hue * 360); // H
	}
}

// Example
// 									
// rgb = { 82, 0, 87 };
// 									
// H: 296
// S: 1
// L: 0.17058824f
