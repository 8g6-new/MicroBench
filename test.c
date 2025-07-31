/*
 * test.c - Example usage of the bench.h single-header benchmark library
 * 
 * Compile with: gcc -o test test.c -lm -lrt
 * Run with: ./test
 */

#define BENCH_IMPLEMENTATION
#include "bench.h"

#include <unistd.h>  // for usleep()

// Example functions to benchmark
void fast_operation(void) {
    volatile int sum = 0;
    for (int i = 0; i < 1000; i++) {
        sum += i * i;
    }
}

void medium_operation(void) {
    volatile double result = 0.0;
    for (int i = 0; i < 50000; i++) {
        result += sin((double)i) * cos((double)i);
    }
}

void slow_operation(void) {
    // Simulate some slow work
    usleep(5000); // 5ms delay
    
    volatile double result = 0.0;
    for (int i = 0; i < 100000; i++) {
        result += sqrt((double)i) / (i + 1.0);
    }
}

void memory_intensive_operation(void) {
    const size_t size = 1000000;
    int *data = malloc(size * sizeof(int));
    
    if (data) {
        // Fill array
        for (size_t i = 0; i < size; i++) {
            data[i] = (int)(i % 1000);
        }
        
        // Sort-like operation
        for (size_t i = 0; i < size - 1; i += 100) {
            if (data[i] > data[i + 1]) {
                int temp = data[i];
                data[i] = data[i + 1];
                data[i + 1] = temp;
            }
        }
        
        free(data);
    }
}

void fft_simulation(void) {
    // Simulate FFT computation workload
    const int N = 1024;
    volatile double real_part = 0.0, imag_part = 0.0;
    
    for (int k = 0; k < N; k++) {
        for (int n = 0; n < N; n++) {
            double angle = -2.0 * M_PI * k * n / N;
            real_part += cos(angle);
            imag_part += sin(angle);
        }
    }
}

int main(void) {
    LOG("Starting benchmark utility test");
    
    // Initialize the benchmark system
    benchmark_init();
    
    printf("\n%s" "═══════════════════════════════════════════════════════════════" "%s\n", 
           BRIGHT_CYAN, RESET);
    printf("%s" "                    BENCHMARK UTILITY TEST                      " "%s\n", 
           BRIGHT_YELLOW, RESET);
    printf("%s" "═══════════════════════════════════════════════════════════════" "%s\n\n", 
           BRIGHT_CYAN, RESET);
    
    // Test 1: Basic timing operations
    printf("%s[TEST 1]%s Basic Operations\n", BRIGHT_GREEN, RESET);
    
    START_TIMING();
    fast_operation();
    END_TIMING("fast_operation");
    
    START_TIMING();
    medium_operation();
    END_TIMING("medium_operation");
    
    START_TIMING();
    slow_operation();
    END_TIMING("slow_operation");
    
    START_TIMING();
    memory_intensive_operation();
    END_TIMING("memory_intensive");
    
    // Test 2: Multiple runs of the same function
    printf("\n%s[TEST 2]%s Multiple Iterations\n", BRIGHT_GREEN, RESET);
    
    for (int i = 0; i < 3; i++) {
        char func_name[64];
        snprintf(func_name, sizeof(func_name), "fast_op_run_%d", i + 1);
        
        START_TIMING();
        fast_operation();
        END_TIMING(func_name);
    }
    
    // Test 3: FFT-specific benchmarking
    printf("\n%s[TEST 3]%s FFT Simulation\n", BRIGHT_GREEN, RESET);
    
    START_TIMING();
    fft_simulation();
    END_TIMING("fft_1024_simulation");
    
    // Get the last recorded time for FFT benchmark display
    benchmark_t* bench = get_bench_instance();
    if (bench->timing_index > 0) {
        double last_time_us = (double)bench->timings[bench->timing_index - 1].time_us;
        FFT_bench(last_time_us, 1024);
    }
    
    // Test 4: Logging examples
    printf("\n%s[TEST 4]%s Logging System\n", BRIGHT_GREEN, RESET);
    
    LOG("This is an informational message with data: %d", 42);
    WARN("This is a warning about potential issue: %.2f", 3.14159);
    ERROR("This is an error message (simulated): %s", "division by zero");
    
    // Test 5: Output different formats
    printf("\n%s[TEST 5]%s Output Formats\n", BRIGHT_GREEN, RESET);
    
    printf("\n%s--- Raw Timing Data ---%s\n", YELLOW, RESET);
    print_bench();
    
    printf("\n%s--- JSON Format ---%s\n", YELLOW, RESET);
    print_bench_json();
    
    printf("\n%s--- Ranked Visualization ---%s\n", YELLOW, RESET);
    print_bench_ranked();
    
    // Test 6: SI scaling demonstration
    printf("\n%s[TEST 6]%s SI Scaling Examples\n", BRIGHT_GREEN, RESET);
    
    double test_values[] = {
        0.000000123,  // nanoseconds
        0.000456,     // microseconds  
        0.123,        // milliseconds
        1.5,          // seconds
        1500.0,       // kiloseconds (uncommon but valid)
        1500000.0,    // megaseconds
        1.5e9,        // gigaseconds
        1.5e12        // teraseconds
    };
    
    const char* test_labels[] = {
        "Very fast operation",
        "Fast operation", 
        "Medium operation",
        "Slow operation",
        "Very slow operation",
        "Extremely slow operation",
        "Geological time operation",
        "Cosmological time operation"
    };
    
    printf("\nSI Scaling Examples:\n");
    for (size_t i = 0; i < sizeof(test_values)/sizeof(test_values[0]); i++) {
        char buffer[64];
        format_scaled(test_values[i], buffer, sizeof(buffer), "s");
        printf("  %-25s: %s\n", test_labels[i], buffer);
    }
    
    // Test 7: Performance measurement
    printf("\n%s[TEST 7]%s Performance Measurement\n", BRIGHT_GREEN, RESET);
    
    const int iterations = 1000;
    
    START_TIMING();
    for (int i = 0; i < iterations; i++) {
        fast_operation();
    }
    END_TIMING("fast_op_1000x");
    
    START_TIMING();
    for (int i = 0; i < iterations / 10; i++) {
        medium_operation();
    }
    END_TIMING("medium_op_100x");
    
    printf("\n%s" "═══════════════════════════════════════════════════════════════" "%s\n", 
           BRIGHT_CYAN, RESET);
    printf("%s" "                        FINAL RESULTS                          " "%s\n", 
           BRIGHT_YELLOW, RESET);
    printf("%s" "═══════════════════════════════════════════════════════════════" "%s\n", 
           BRIGHT_CYAN, RESET);
    
    print_bench_ranked();
    
    printf("\n%s✅ Benchmark utility test completed successfully!%s\n\n", 
           BRIGHT_GREEN, RESET);
    
    return 0;
}