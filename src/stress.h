#ifndef CPU_BURNER_STRESS_H_
#define CPU_BURNER_STRESS_H_

#include "framework.h"

extern long long stress_prime_result;

namespace comp {

 void heavy_computation();

 void avx_computation();

 void light_computation();

}

void start_cpu_stress(int num_threads);

void stop_cpu_stress();

extern bool _run_state;

void set_run_state(bool on);

bool test_stress(const int num_threads, unsigned long num_ms);

long long launch_stressors(const int num_threads, unsigned long num_ms);

#endif // CPU_BURNER_STRESS_H_
