#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[]) {
	
	long n=1e8;
	int k, thread_count=4;
	long double sum1 = 1.0, sum2 = 1.0;

	
	// Serial
    	double t0 = omp_get_wtime();
    	

        for (k = 0; k < n; k++) {
            sum1 *= (1 + (1.0/(4*k+1)))*(1-(1.0/(4*k+3)));
        }
        
        
	double t1 = omp_get_wtime();
	
	
	// Parallel
	
	double t2 = omp_get_wtime();
    	
    	#pragma omp parallel for num_threads(thread_count) \
    	default(none) \
    	shared(n) private(k) \
    	reduction(* : sum2)
        for (k = 0; k < n; k++) {
            sum2 *= (1 + (1.0/(4*k+1)))*(1-(1.0/(4*k+3)));
        }
        
	double t3 = omp_get_wtime();
	
	
	printf("\nSerial Result:               %.12Lf\n", sum1);
	printf("Parallel Result (Threads=%d): %.12Lf\n", thread_count, sum2);
	printf("Real Value:                  %.12f\n\n", sqrt(2));
	printf("Serial Time:               %.5f\n", t1-t0);
	printf("Parallel Time (Threads=%d): %.5f\n\n", thread_count, t3-t2);

	return 0;
        
}
