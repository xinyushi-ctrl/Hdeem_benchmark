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
//    printf(HLINE);
#pragma omp parallel
    {
#pragma omp master
        {
          int  k = omp_get_num_threads();
            printf ("Number of Threads requested = %i\n",k);
        }
    }
#endif


    long j;

    double *a , *b , *c;
    a = (double*)malloc(ARRAY_SIZE * sizeof(double));
    b = (double*)malloc(ARRAY_SIZE * sizeof(double));
    c = (double*)malloc(ARRAY_SIZE * sizeof(double));

    int i;

    a[0] = 1.0; b[0] = 2.0; c[0] = 0.0;
    a = &a[0];
    b = &b[0];
    c = &c[0];
    struct timeval start;
    struct timeval end;
    unsigned long timer;
    double duration;
    double energy, aver_power;

#pragma omp parallel for
    for (j = 1; j < ARRAY_SIZE; j++){

            *(a+j) = 2.0E0 * (*(a+j-1));

        }
    for (j = 1; j < ARRAY_SIZE; j++)
            *(b+j) = 2.0E0 * (*(b+j-1)) + 1;

    hdeem_bmc_data_t bmc;
    int rc;
    hdeem_stats_reading_t stats;


    bmc.host = NULL;
    rc = hdeem_init(&bmc);
    if(rc){
        printf("hdeem_init failed!\n");
        return rc;
    }

    rc = hdeem_get_stats_total(&bmc, &stats);
    double blade_1 , vr_1[5];
    printf("\n");
    for (i = 0; i < bmc.nb_blade_sensors; i++){
        printf("%s   ", bmc.name_blade_sensors[i]);
        printf("Accumulated Energy %12.3f J\n", stats.energy_blade_values.value[i]);
        blade_1 = stats.energy_blade_values.value[i];
    }

    for (i = 0; i < bmc.nb_vr_sensors; i++){
        printf("%s   ", bmc.name_vr_sensors[i]);
        printf("Accumulated Energy %12.3f J\n", stats.energy_vr_values.value[i]);
        vr_1[i] = stats.energy_vr_values.value[i];
    }


    printf("\n");

    gettimeofday(&start,NULL);

    for (int t = 0; t < 5; t++)
        addition(a, b, c);

    gettimeofday(&end,NULL);

    rc = hdeem_get_stats_total(&bmc, &stats);
    double blade_2 , vr_2[5];
    printf("\n");
    for (i = 0; i < bmc.nb_blade_sensors; i++){
        printf("%s   ", bmc.name_blade_sensors[i]);
        printf("Accumulated Energy %12.3f J\n", stats.energy_blade_values.value[i]);
        blade_2 =  stats.energy_blade_values.value[i];
    }

    for (i = 0; i < bmc.nb_vr_sensors; i++){
        printf("%s   ", bmc.name_vr_sensors[i]);
        printf("Accumulated Energy %12.3f J\n", stats.energy_vr_values.value[i]);
        vr_2[i] = stats.energy_vr_values.value[i];
    }

    printf("\n");

    fflush(stdout);

    free(a);
    free(b);
    free(c);

    hdeem_stats_free(&stats);

    hdeem_close(&bmc);
    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    printf("timer = %ld us\n", timer);
    duration = timer/1000000;
    printf("\n");

    for (i = 0; i < bmc.nb_blade_sensors; i++){
        printf("%s   ", bmc.name_blade_sensors[i]);
        printf("Average Power: %8fW\n", (blade_2 - blade_1)/duration);

    }

    for (i = 0; i < bmc.nb_vr_sensors; i++){
        printf("%s   ", bmc.name_vr_sensors[i]);
        printf("Average Power: %8fW\n", (vr_2[i] - vr_1[i])/duration);
    }

    printf("\n");


    return 0;
}

void addition(double* a, double* b, double* c){
    long k;
#pragma omp parallel for
    for ( k = 0; k < ARRAY_SIZE; k+=8)
            *(c+k) = *(a+k) + *(b+k);
}