#include <stdio.h>
#include "hdeem.h"
#include <time.h>
#include <stdlib.h>

#define ARRAY_SIZE 100000000


void addition(double* a, double* b, double* c);

int main(void){
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

    addition(a, b, c);
    rc = hdeem_stop(&bmc);
    free(a);
    free(b);
    free(c);

    rc = hdeem_get_stats(&bmc, &stats);

    printf("\n");
    for (i = 0; i < bmc.nb_blade_sensors; i++){
        printf("%s   ", bmc.name_blade_sensors[i]);
        printf("The average power :%8.3f W  ", stats.average_blade_values.value[i]);
        printf("Max power %8.3f  ", stats.max_blade_values.value[i]);
        printf("Min power %8.3f\n", stats.min_blade_values.value[i]);
    }

    for (i = 0; i < bmc.nb_vr_sensors; i++){
        printf("%s   ", bmc.name_vr_sensors[i]);
        printf("The average power:%8.3f W  ", stats.average_vr_values.value[i]);
        printf("Max power %8.3f  ", stats.max_vr_values.value[i]);
        printf("Min power %8.3f\n", stats.min_vr_values.value[i]);
    }

    printf("\n");
    fflush(stdout);

    hdeem_stats_free(&stats);



    hdeem_close(&bmc);


    return 0;
}

void addition(double* a, double* b, double* c){
    long k;
    for ( k = 0; k < ARRAY_SIZE; k+=8)
            *(c+k) = *(a+k) + *(b+k);

}