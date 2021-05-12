#include <stdio.h>
#include "hdeem.h"
#include <time.h>
#include <stdlib.h>
#include <omp.h>

#define ARRAY_SIZE 100000000

#ifdef _OPENMP
extern int omp_get_num_threads();
#endif

void addition(double* a, double* b, double* c);

int main(void){

#ifdef _OPENMP
#pragma omp parallel
    {
#pragma omp master
        {
          int  k = omp_get_num_threads();
            printf ("Number of Threads requested = %i\n",k);
        }
    }
#endif
printf("Test0");
    long j;

    double *a = (double*)malloc(ARRAY_SIZE * sizeof(double));
    double *b = (double*)malloc(ARRAY_SIZE * sizeof(double));
    double *c = (double*)malloc(ARRAY_SIZE * sizeof(double));
        printf("point 2");
    int i;

    a[0] = 1.0; b[0] = 2.0; c[0] = 0.0;
        printf("point3");

    struct timeval start;
    struct timeval end;
    unsigned long timer;
    double duration;
//    double energy, aver_power;
#pragma omp parallel for
    for (j = 1; j < ARRAY_SIZE; j++){

            *(a+j) = 2.0E0 * (*(a+j-1));
        }
        printf("point4");
    for (j = 1; j < ARRAY_SIZE; j++)
            *(b+j) = 2.0E0 * (*(b+j-1)) + 1;
        printf("point5");

    hdeem_bmc_data_t bmc;
    int rc;
    hdeem_stats_reading_t stats;

    bmc.host = NULL;
    rc = hdeem_init(&bmc);
    if(rc){
        printf("hdeem_init failed!\n");
        return rc;
    }

    if(hdeem_start(&bmc)){
        printf("first hdeem_start failed, try hdeem_clear!\n");
        rc = hdeem_clear(&bmc);
        if(rc){
        printf("hdeem_clear failed!\n");
        return rc;
    }

    rc = hdeem_start(&bmc);
    if(rc){
        printf("hdeem_start failed!\n");
        return rc;
    }
  }
    gettimeofday(&start,NULL);

    for (int t = 0; t < 5; t++)
        addition(a,b,c);//main loop

    gettimeofday(&end,NULL);

    rc = hdeem_stop(&bmc);

    free(a);
    free(b);
    free(c);

    rc = hdeem_get_stats(&bmc, &stats);
    double blade_power;
    //double extra_power;

    printf("\n");
    for (i = 0; i < bmc.nb_blade_sensors; i++){
        printf("%s   ", bmc.name_blade_sensors[i]);
        printf("The average power :%8.3f W  ", stats.average_blade_values.value[i]);
        printf("Max power %8.3f  ", stats.max_blade_values.value[i]);
        printf("Min power %8.3f  ", stats.min_blade_values.value[i]);
        printf("Energy %12.3f\n", stats.energy_blade_values.value[i]);
        blade_power = stats.average_blade_values.value[i];
    }

    for (i = 0; i < bmc.nb_vr_sensors; i++){
        printf("%s   ", bmc.name_vr_sensors[i]);
        printf("The average power:%8.3f W  ", stats.average_vr_values.value[i]);
        printf("Max power %8.3f  ", stats.max_vr_values.value[i]);
        printf("Min power %8.3f  ", stats.min_vr_values.value[i]);
        printf("Energy %12.3f\n", stats.energy_vr_values.value[i]);
        blade_power = blade_power - stats.average_vr_values.value[i];

        }
    printf("The extra average power of the system: %lf\n",blade_power);
    printf("\n");
    fflush(stdout);

//    printf("The extra average power of the system: %lf\n",extra_power);

    hdeem_stats_free(&stats);

    hdeem_close(&bmc);


    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    printf("timer = %ld us\n", timer);
    return 0;
}

void addition(double* a, double* b, double* c){
    long k;
#pragma omp parallel for
    for ( k = 0; k < ARRAY_SIZE; k+=8)
            *(c+k) = *(a+k) + *(b+k);
}
