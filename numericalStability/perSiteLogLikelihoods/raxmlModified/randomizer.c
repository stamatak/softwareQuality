#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <inttypes.h>
#include <assert.h>


double randum (int64_t  *seed)
{
  int64_t  sum, mult0, mult1, seed0, seed1, seed2, newseed0, newseed1, newseed2;
  double res;

  mult0 = 1549;
  seed0 = *seed & 4095;
  sum  = mult0 * seed0;
  newseed0 = sum & 4095;
  sum >>= 12;
  seed1 = (*seed >> 12) & 4095;
  mult1 =  406;
  sum += mult0 * seed1 + mult1 * seed0;
  newseed1 = sum & 4095;
  sum >>= 12;
  seed2 = (*seed >> 24) & 255;
  sum += mult0 * seed2 + mult1 * seed1;
  newseed2 = sum & 255;

  *seed = newseed2 << 24 | newseed1 << 12 | newseed0;
  res = 0.00390625 * (newseed2 + 0.000244140625 * (newseed1 + 0.000244140625 * newseed0));

  return res;
}

void makePermutation(int *perm, int lower, int n, int64_t *seed)
{    
  int  i, j, k;

  for (i = lower; i <= n; i++)    
    perm[i] = i;               

  for (i = lower; i <= n; i++) 
    {    
      k =  (int)((double)(n + 1 - i) * randum(seed));

      assert(i + k <= n);
      
      j        = perm[i];
      perm[i]     = perm[i + k];
      perm[i + k] = j; 
    }
}


int main (int argc, char *argv[])
{
  int 
    i,
    j,
    l,
    length;
  
  int64_t 
    seed = 12345;
  
  FILE *f = fopen("ziheng.in", "rb");

  double *a;



  int *p;

  fread(&length, sizeof(int), 1, f);

 

  a = (double *)malloc(sizeof(double) * length);
 

  fread(a, sizeof(double), length, f);

  //for(i = 0; i < length; i++)
  //  printf("%f \n", a[i]);

  for(l = 1; l < 256; l *= 2)
    {
      double 
	*v = (double *)malloc(sizeof(double) * l * length),
	min = 10.0,
	max = -1.0E300;
      
      p = (int *)malloc(sizeof(int) * (l * length + 1));
      
      printf("members :%d \n", l * length);

      for(i = 0; i < l; i++)
	memcpy(&v[i * length], a, sizeof(double) * length);
      
      for(i = 0; i < 1000; i++)
	{
	  double 
	    sum = 0.0;

	  makePermutation(p, 1, l * length, &seed);

	  for(j = 1; j <= l * length; j++)
	    {
	      int 
		index = p[j] - 1;
	      
	      assert(index >= 0 && index < l * length);
	      
	      sum += v[index];
	    }

	  if(sum < min)
	    min = sum;
	  if(sum > max)
	    max = sum;

	  //printf("%8.20f\n", sum);
	}
      printf("max %8.20f min %8.20f diff %8.20f\n", max, min, fabs(max - min));
      free(v);
      free(p);
    }

  

  fclose(f);
}
