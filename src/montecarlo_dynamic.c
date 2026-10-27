
/* montecarlo_paralelo.c */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>

#define SEMILLA_BASE UINT64_C(2211714)

/* Mezcla determinista de 64 bits. */
static uint64_t mezclar(uint64_t x) {
    x += UINT64_C(0x9E3779B97F4A7C15);
    x = (x ^ (x >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    x = (x ^ (x >> 27)) * UINT64_C(0x94D049BB133111EB);
    return x ^ (x >> 31);
}

/* Valor pseudoaleatorio uniforme en [0,1). */
static double uniforme(uint64_t indice) {
    uint64_t r = mezclar(SEMILLA_BASE + indice);
    return (double)(r >> 11) * (1.0 / 9007199254740992.0);
}

int main(int argc, char *argv[]) {
    long N = (argc > 1) ? atol(argv[1]) : 100000000L;
    int p = (argc > 2) ? atoi(argv[2]) : 1;

    if (N <= 0 || p <= 0) {
        fprintf(stderr, "Uso: %s muestras hilos\n", argv[0]);
        return 1;
    }

    long long dentro = 0;
    omp_set_num_threads(p);

    double inicio = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic) reduction(+:dentro)
    for (long i = 0; i < N; i++) {
        uint64_t indice = (uint64_t)i;
        double x = uniforme(2 * indice);
        double y = uniforme(2 * indice + 1);

        if (x * x + y * y <= 1.0) {
            dentro++;
        }
    }

    double fin = omp_get_wtime();

    printf("Muestras: %ld\n", N);
    printf("Hilos solicitados: %d\n", p);
    printf("Aciertos: %lld\n", dentro);
    printf("Estimacion de pi: %.10f\n",
           4.0 * (double)dentro / (double)N);
    printf("Tiempo paralelo: %.6f segundos\n", fin - inicio);

    return 0;
}
