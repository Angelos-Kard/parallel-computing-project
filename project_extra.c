#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[]) {
	
	long n=1e6;
	int k, mode, thread_count=4;
	long double sum1 = 1.0, sum2 = 1.0;
	long double sum_arr[3];
	double t_arr[6], t2, t3;
	
	
	if (argc > 1) // Set custom n. If n=0, then the argument is ignored
	{
	  if (atol(argv[1]) != 0) n = atoi(argv[1]);
	}
	if (argc > 2) // Set 
	{
	  mode = atoi(argv[2]);
	  
	  if (mode == 0) // mode = 0: Set custom thread_count
	  {
	    if (argc > 3)
	    {
	      if (atoi(argv[3]) != 0) thread_count = atoi(argv[3]);
	      else
	      {
	        printf("You can't set thread_count = 0");
	    	return -1;
	      }
	    }	    
	  }
	  else if (mode != 1) // Available modes: 0, 1
	  {
	      printf("Wrong mode input.\nAvailable modes:\n");
	      printf("- 0: Set custom thread_count\n");
	      printf("- 1: Run the program for thread_count = 2, 4 and 8\n");
	      printf("If the mode isn't defined, then the program will run with thread_count = 4");
	      return -1;
	  }
	}
	else mode = 0;

	
	// Serial
    	double t0 = omp_get_wtime();
    	

        for (k = 0; k < n; k++) {
            sum1 *= (1 + (1.0/(4*k+1)))*(1-(1.0/(4*k+3)));
        }
        
        
	double t1 = omp_get_wtime();
	
	
	// Parallel
	
	if (mode != 1)
	{
	
	t2 = omp_get_wtime();
    	
    	#pragma omp parallel for num_threads(thread_count) \
    	default(none) \
    	shared(n) private(k) \
    	reduction(* : sum2)
        for (k = 0; k < n; k++) {
            sum2 *= (1 + (1.0/(4*k+1)))*(1-(1.0/(4*k+3)));
        }
        
	t3 = omp_get_wtime();
	
	}
	else if (mode == 1) // Run the program for thread_count=2,4,8
	{
	  int counter = 0, ind = 0;
	  
	  for (int t = 2; t <= 8; t*=2)
	  {
	    t_arr[counter] = omp_get_wtime();
	    
    	    sum_arr[ind] = 1.0;
    	    #pragma omp parallel for num_threads(t) \
    	    default(none) \
    	    shared(n, ind) private(k) \
    	    reduction(* : sum_arr[ind])
            for (k = 0; k < n; k++) {
              sum_arr[ind] *= (1 + (1.0/(4*k+1)))*(1-(1.0/(4*k+3)));
            }
        
	    t_arr[counter+1] = omp_get_wtime();
	    counter += 2;
	    ind++;
	  }
	}
	
	printf("\nSerial Result:               %.12Lf\n", sum1);
	if (mode == 0) printf("Parallel Result (Threads=%d): %.12Lf\n", thread_count, sum2);
	else if (mode == 1)
	{
	printf("Parallel Result (Threads=%d): %.12Lf\n", 2, sum_arr[0]);
	printf("Parallel Result (Threads=%d): %.12Lf\n", 4, sum_arr[1]);
	printf("Parallel Result (Threads=%d): %.12Lf\n", 8, sum_arr[2]);
	}
	printf("Real Value:                  %.12f\n\n", sqrt(2));
	printf("Serial Time:               %.5f\n", t1-t0);
	if (mode == 0) printf("Parallel Time (Threads=%d): %.5f\n\n", thread_count, t3-t2);
	else if (mode == 1)
	{
	printf("Parallel Time (Threads=%d): %.5f\n", 2, t_arr[1] - t_arr[0]);
	printf("Parallel Time (Threads=%d): %.5f\n", 4, t_arr[3] - t_arr[2]);
	printf("Parallel Time (Threads=%d): %.5f\n\n", 8, t_arr[5] - t_arr[4]);
	}
	
	
	
	
	
	return 0;
        
}
