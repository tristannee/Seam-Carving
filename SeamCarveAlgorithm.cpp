/*
 * SeamCarveAlgorithm.cpp
 * Defines the seam carving algorithm.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

#include "SeamCarveApp.hpp"
#include <iostream>

#define min(x, y)           ((x) < (y) ? (x) : (y))

using namespace std;

int min3(int x, int y, int z)
{
	int min = x;
	
	if (y < min)
		min = y;
	if (z < min)
		min = z;
		
	return min;
}

/**
 * @brief Peforms the seam carving algorithm.
 *
 * @param smap 2-d saliency map with width `w` and height `h`; can be
 * indexed by `smap[i][j]`
 *
 * @param w Width of the saliency map
 *
 * @param h Height of the saliency map
 */
unsigned int *DoSeamCarve(unsigned int **smap, int w, int h)
{ 
    unsigned int *seam = new unsigned int[h];
    int **cmap = new int*[w]; //cost map
    
    for (int i = 0; i < h; i++)
        seam[i] = 0;
    
    for (int a = 0; a < w; a++)
		cmap[a] = new int[h];		
		
	for (int a = 0; a < w; a++) //set first row of cost map
		cmap[a][0] = smap[a][0];
	for (int b = 1; b < h; b++) //set remaning rows of cost map
	{
		for (int a = 0; a < w; a++)
			cmap[a][b] = smap[a][b];
	}

	for (int b = 1; b < h; b++) //calculate values of cost map
	{
		for (int a = 0; a < w; a++)
		{
			if (a != 0 && a != w - 1)
				cmap[a][b] += min3(cmap[a - 1][b - 1], cmap[a][b - 1], cmap[a + 1][b - 1]);
			else if (a == 0)
				cmap[a][b] += min(cmap[a][b - 1], cmap[a + 1][b - 1]);
			else
				cmap[a][b] += min(cmap[a - 1][b - 1], cmap[a][b - 1]);
		}
	}
	int min = cmap[0][h - 1]; //initial minimum value
	int minc = 0; //x coordinate corresponding to the minimum value
	for (int a = 0; a < w; a++)
	{
		if (cmap[a][h - 1] < min)
		{
			min = cmap[a][h - 1];
			minc = a;
		}
	}
	
	seam[h - 1] = minc;
	
	for (int b = h - 2; b >= 0; b--)
	{
		if (minc != 0 && minc != w - 1)
		{
			if (cmap[minc - 1][b] < cmap[minc][b] && cmap[minc - 1][b] < cmap[minc + 1][b])
				minc -= 1; 	//diagonally to the left is the best location
			else if (cmap[minc + 1][b] < cmap[minc - 1][b] && cmap[minc + 1][b] < cmap[minc][b])
				minc += 1; //diagonally to the right is the best location
		}
		else if (minc == 0)
		{
			if (cmap[minc + 1][b] < cmap[minc][b])
				minc += 1;
		}
		else
		{
			if (cmap[minc - 1][b] < cmap[minc][b])
				minc -= 1;
		}
		seam[b] = minc;
	}

    
    for (int a = 0; a < w; a++)
		delete[] cmap[a];
	delete cmap;

    return seam;
}

