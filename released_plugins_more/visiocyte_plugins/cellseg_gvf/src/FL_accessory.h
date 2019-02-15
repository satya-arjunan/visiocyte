// accessory functions for get info from image matrix
// F. Long
// 20080510

#ifndef __FL_ACCESSORY__
#define __FL_ACCESSORY__

#include "FL_defType.h"

// Convert from linear index to array coordinates. 
template <class T> void ind_to_sub(VISIOCYTELONG p, T in_num_dims, VISIOCYTELONG *cumprod, VISIOCYTELONG *coords)
{
    int j;
    
    for (j = (int)in_num_dims-1; j >= 0; j--)
    {
        coords[j] = p / cumprod[j];
        p = p % cumprod[j];
    }
}

// Convert from array coordinates to zero-based linear index.  
template <class T> VISIOCYTELONG sub_to_ind(VISIOCYTELONG *coords, VISIOCYTELONG *cumprod, T in_num_dims)
{
    VISIOCYTELONG index = 0;
    int k;

    for (k = 0; k < int(in_num_dims); k++)
    {
        index += coords[k] * cumprod[k];
    }

    return index;
}


// Compute linear relative offset when given relative array offsets.
template <class T> VISIOCYTELONG sub_to_relative_ind(VISIOCYTELONG *coords, T in_num_dims)
{
    int N;
    int P;
    int abs_coord;
    VISIOCYTELONG cumprod = 1;
    int index = 0;
    int k;

     // Find the maximum absolute neighbor offset.
    N = 0;
    for (k = 0; k < int(in_num_dims); k++)
    {
        abs_coord = coords[k] > 0 ? coords[k] : -coords[k];
        if (abs_coord > N)
        {
            N = abs_coord;
        }
    }
    P = 2*N + 1;

    // Perform sub_to_ind computation.
    for (k = 0; k < int(in_num_dims); k++)
    {
        index += coords[k] * cumprod;
        cumprod *= P;
    }

    return index;
}


 // Count nonzero elements 

template <class T> VISIOCYTELONG num_nonzeros(const T *D, const VISIOCYTELONG num_elements)
{
    int p;
    VISIOCYTELONG count = 0;
		
	for (p = 0; p < num_elements; p++)
	{ 
	  if (D[p]) count++; 
	}
    
    return count;
}

#endif