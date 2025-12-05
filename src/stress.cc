#include "stress.h"

std::atomic<bool> running(true); // Flag to control the thread execution

bool _run_state = false;

long long stress_prime_result;

// A CPU-intensive function that performs a heavy calculation
void comp::heavy_computation() {
  while (running) {
    unsigned long long num = 4096L;
    bool is_prime = true;

    // A simple prime number generator (which will use a lot of CPU)
    for (unsigned long long i = 2; i < num; ++i) {
      if (num % i == 0) {
        is_prime = false;
        break;
      }
    }

    if (is_prime) {
      // If prime, add 1 and continue prime search
      num = num + 1;
    }

    stress_prime_result = num;
  }
}

void comp::avx_computation() {
}

void comp::light_computation() {
}

// Function to start the stress test with multiple threads
void start_cpu_stress(int num_threads) {
  std::vector<std::thread> threads;

  // Create threads
  for (int i = 0; i < num_threads; ++i) {
    threads.push_back(std::thread(comp::heavy_computation));
  }

  // Wait for threads to finish (this will never happen unless `running` is set to false)
  for (auto& thread : threads) {
    thread.join();
  }
}

// Function to stop the CPU stress test
void stop_cpu_stress() {
  static const wchar_t* buffer = L"Stopping threads...";
  std::wcout << buffer << std::endl;
  running = false;
  _run_state = false;
}

void set_run_state(bool on) {
  const bool is_on = on;
  if (!is_on) {
    stop_cpu_stress();
  } else {
    _run_state = is_on;
  }
}

bool test_stress(const int num_threads, unsigned long num_ms) {
  bool success = false;

  static const float num_sec = static_cast<float>(num_ms) / 1000.0f;
  static const std::wstring sec_str =
      num_sec == 1.0f ? L" second" : L" seconds";
  std::wcout << std::fixed << std::setprecision(4)
             << L"Starting CPU stress test with "
             << num_threads << L" threads for " 
             << num_sec << sec_str << std::endl;

  std::thread start_stress(launch_stressors, num_threads, num_ms);
  start_stress.detach();

  std::wcout << __FUNC__ << L" completed." << std::endl;

  return success;
}

long long launch_stressors(const int num_threads, unsigned long num_ms) {
  if (num_threads == 0) {
    return 0LL;
  }
  const std::chrono::duration stress_time = std::chrono::milliseconds(num_ms);

  // Start the stress test in a separate thread or in main
  set_run_state(true);
  std::thread stress_thread(start_cpu_stress, num_threads);

  // Let it run for a while and then stop it
  std::this_thread::sleep_for(stress_time);  // Stress for 1 seconds

  set_run_state(false);
  stress_thread.join(); // Make sure to join the stress thread before exiting
  return stress_prime_result;
}
