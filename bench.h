#ifndef BENCH_H
    #define BENCH_H

    #include <time.h>
    #include <stdio.h>
    #include <stddef.h>
    #include <stdlib.h>
    #include <string.h>
    #include <math.h>

    /*
    * The MIT License (MIT)
    * 
    * Copyright © 2025 Devadut S Balan
    * 
    * Permission is hereby granted, free of charge, to any person obtaining a copy
    * of this software and associated documentation files (the "Software"), to deal
    * in the Software without restriction, including without limitation the rights
    * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    * copies of the Software, and to permit persons to whom the Software is
    * furnished to do so, subject to the following conditions:
    * 
    * The above copyright notice and this permission notice shall be included in
    * all copies or substantial portions of the Software.
    * 
    * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    * SOFTWARE.
    */

    /**
    * @file bench.h
    * @brief Single-header benchmarking and value-scaling utilities using general-purpose SI prefixes.
    * @author Devadut S Balan
    * @license MIT
    * 
    * Usage:
    *   #define BENCH_IMPLEMENTATION
    *   #include "bench.h"
    */

    #define MAX_FUNS_TO_BENCH      600     /**< Maximum number of benchmarked functions. */
    #define MAX_FUNS_NAME_LENGTH   100     /**< Maximum characters in a function label. */
    #define BAR_LENGTH             20      /**< Width of progress bars in ranking output. */

    // ─── ANSI Colors ──────────────────────────────────────────────────────────────
    #define RESET           "\x1b[0m"
    #define BRIGHT_RED      "\x1b[91m"
    #define RED             "\x1b[31m"
    #define MAGENTA         "\x1b[35m"
    #define BRIGHT_YELLOW   "\x1b[93m"
    #define YELLOW          "\x1b[33m"
    #define BRIGHT_GREEN    "\x1b[92m"
    #define GREEN           "\x1b[32m"
    #define BLUE            "\x1b[34m"
    #define BRIGHT_CYAN     "\x1b[96m"
    #define BRIGHT_BLUE     "\x1b[94m"
    #define BAR_COLOR       BRIGHT_BLUE

    /**
    * @brief Unicode bar used for visual separation in terminal.
    */
    static const char line[] =
    "\n▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰\n";

    /**
    * @brief SI unit prefix scale (for time, data, size, etc.)
    */
    typedef struct {
        const char *suffix;       /**< SI unit suffix ("n", "µ", "m", "", "k", etc.) */
        double scale_divisor;     /**< Numerical divisor to scale a raw value */
    } scale;

    /**
    * @brief Enum index for each supported SI scale.
    */
    typedef enum {
        scale_nano_idx = 0,   /**< nano (1e-9) */
        scale_micro_idx,      /**< micro (1e-6) */
        scale_milli_idx,      /**< milli (1e-3) */
        scale_unit_idx,       /**< base (1e0) */
        scale_kilo_idx,       /**< kilo (1e3) */
        scale_mega_idx,       /**< mega (1e6) */
        scale_giga_idx,       /**< giga (1e9) */
        scale_tera_idx,       /**< tera (1e12) */
        scale_count           /**< Total number of scales */
    } scale_index;

    /**
    * @brief General-purpose SI scaling table.
    *
    * These scales can be used for:
    * - Time (e.g., µs, ms)
    * - Memory (e.g., kB, MB)
    * - Frequency or rates (e.g., MFLOP/s, MHz)
    * - Any measurable quantity with power-of-ten representation
    */
    static const scale scales[scale_count] = {
        { "n", 1e-9 },
        { "µ", 1e-6 },
        { "m", 1e-3 },
        { "",  1    },
        { "k", 1e3  },
        { "M", 1e6  },
        { "G", 1e9  },
        { "T", 1e12 }
    };

    #define STRING_LENGTH 32

    /**
    * @brief Benchmark info for a single function.
    */
    typedef struct {
        long long time_us;                            /**< Execution time in microseconds */
        char function_name[MAX_FUNS_NAME_LENGTH];     /**< Human-readable function label */
    } time_info;

    /**
    * @brief Stores timing data for all benchmarked functions.
    */
    typedef struct {
        long long  total_time;                        /**< Total time recorded */
        long long  start_time;                        /**< Internal use only */
        time_info  timings[MAX_FUNS_TO_BENCH];        /**< Per-function timing info */
        size_t     timing_index;                      /**< Number of functions tracked */
    } benchmark_t;

    // ─── Function Declarations ───────────────────────────────────────────────────

    /**
    * @brief Initialize the global benchmark state.
    */
    void benchmark_init(void);

    /**
    * @brief Returns current high-resolution time in microseconds.
    * @return Time in µs.
    */
    long long get_time_us(void);

    /**
    * @brief Records the time delta since `START_TIMING()` under a named function.
    * @param function_name Label to assign to the recorded time.
    */
    void record_timing(const char *function_name);

    /**
    * @brief Compare two time_info entries (descending).
    * @param a Pointer to first time_info.
    * @param b Pointer to second time_info.
    * @return Sorting order: negative if a < b.
    */
    int compare_times(const void *a, const void *b);

    /**
    * @brief Choose the most human-readable SI scale for a value.
    * @param v Raw value (e.g., 2500000.0).
    * @return Matching scale struct with suffix and divisor.
    */
    scale get_scale(double v);

    /**
    * @brief Format a scaled value into a human-readable string.
    * @param val       Raw value to scale.
    * @param buffer    Output string buffer.
    * @param buf_size  Maximum buffer length.
    * @param unit      Unit string (e.g., "s", "FLOP/s").
    */
    void format_scaled(double val, char *buffer, size_t buf_size, const char *unit);

    /**
    * @brief Estimate MFLOP/s for a given FFT and print timing and throughput.
    * @param mu_s      Average elapsed time per FFT in microseconds.
    * @param FFT_size  Number of FFT points (N).
    */
    void FFT_bench(double mu_s, unsigned int FFT_size);

    /**
    * @brief Choose a display color for ranking output based on percentage.
    * @param percentage Time or value percentage (0.0 to 100.0)
    * @return ANSI color string.
    */
    const char* get_gradient_color(double percentage);

    /**
    * @brief Print raw timings for all benchmarked functions.
    */
    void print_bench(void);

    /**
    * @brief Print all benchmark data as a JSON object.
    */
    void print_bench_json(void);

    /**
    * @brief Pretty-print ranked benchmark chart in table and bar format.
    */
    void print_bench_ranked(void);

    // ─── Macros ───────────────────────────────────────────────────────────────────

    /**
    * @brief Start timing a block of code.
    */
    #define START_TIMING()           (get_bench_instance()->start_time = get_time_us())

    /**
    * @brief Stop timing and record elapsed duration.
    * @param FUNC_NAME Label to assign this timed block.
    */
    #define END_TIMING(FUNC_NAME)    record_timing(FUNC_NAME)

    /**
    * @brief Print an informational message with file, function, and line metadata.
    *
    * Outputs to stdout with a bright cyan [INFO] label and contextual metadata
    * including source file, function name, and line number.
    *
    * @param ... A printf-style format string followed by optional arguments.
    *
    * @example
    * LOG("Loaded %zu samples from %s", num_samples, filename);
    */
    #define LOG(...) \
        do { \
            fprintf(stdout, BRIGHT_CYAN "[INFO] " RESET "[file: %s | line: %d | func: %s] ", __FILE__, __LINE__, __func__); \
            fprintf(stdout, __VA_ARGS__); \
            fprintf(stdout, "\n"); \
        } while (0)

    /**
    * @brief Print a warning message with file, function, and line metadata.
    *
    * Outputs to stderr with a bright yellow [WARN] label and contextual metadata
    * including source file, function name, and line number.
    *
    * @param ... A printf-style format string followed by optional arguments.
    *
    * @example
    * WARN("Fallback to default window type: %s", fallback_type);
    */
    #define WARN(...) \
        do { \
            fprintf(stderr, BRIGHT_YELLOW "[WARN] " RESET "[file: %s | line: %d | func: %s] ", __FILE__, __LINE__, __func__); \
            fprintf(stderr, __VA_ARGS__); \
            fprintf(stderr, "\n"); \
        } while (0)

    /**
    * @brief Print an error message with file, function, and line metadata.
    *
    * Outputs to stderr with a bright red [ERROR] label and contextual metadata
    * including source file, function name, and line number.
    *
    * @param ... A printf-style format string followed by optional arguments.
    *
    * @example
    * ERROR("Memory allocation failed for %zu bytes", buffer_size);
    */
    #define ERROR(...) \
        do { \
            fprintf(stderr, BRIGHT_RED "[ERROR] " RESET "[file: %s | line: %d | func: %s] ", __FILE__, __LINE__, __func__); \
            fprintf(stderr, __VA_ARGS__); \
            fprintf(stderr, "\n"); \
        } while (0)

    /**
    * @brief Get pointer to global benchmark instance.
    * @return Pointer to benchmark_t instance.
    */
    benchmark_t* get_bench_instance(void);

    // ═══════════════════════════════════════════════════════════════════════════════
    // ███████████████████████ IMPLEMENTATION SECTION ███████████████████████████████
    // ═══════════════════════════════════════════════════════════════════════════════

        #ifdef BENCH_IMPLEMENTATION

        static benchmark_t benchmarks;

        benchmark_t* get_bench_instance(void) {
            return &benchmarks;
        }

        void benchmark_init(void) {
            benchmarks.total_time = 0;   
            benchmarks.timing_index = 0; 
            benchmarks.start_time = 0;
        }

        long long get_time_us(void) {
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC, &ts);
            return (long long)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
        }

        void record_timing(const char *function_name) {
            if (benchmarks.timing_index >= MAX_FUNS_TO_BENCH) {
                fprintf(stderr, "Error: Exceeded maximum number of benchmarked functions!\n");
                return;
            }

            long long end_time = get_time_us();
            benchmarks.timings[benchmarks.timing_index].time_us = end_time - benchmarks.start_time;
            benchmarks.total_time += benchmarks.timings[benchmarks.timing_index].time_us;

            strncpy(benchmarks.timings[benchmarks.timing_index].function_name, 
                    function_name, 
                    sizeof(benchmarks.timings[benchmarks.timing_index].function_name) - 1);
            
            benchmarks.timings[benchmarks.timing_index].function_name[
                sizeof(benchmarks.timings[benchmarks.timing_index].function_name) - 1] = '\0';

            benchmarks.timing_index++;
        }

        scale get_scale(double v) {
            if (v == 0.0) {
                return scales[scale_nano_idx]; 
            }
            for (int i = scale_count - 1; i >= 0; --i) {
                double scaled = v / scales[i].scale_divisor;
                if (fabs(scaled) >= 1.0) {
                    return scales[i];
                }
            }
            return scales[scale_nano_idx]; 
        }

        void format_scaled(double val, char *buffer, size_t buf_size, const char *unit) {
            scale s = get_scale(val);
            double scaled = val / s.scale_divisor;
            snprintf(buffer, buf_size, "%7.3f %s%s", scaled, s.suffix, unit);
        }

        void FFT_bench(double mu_s, unsigned int FFT_size) {
            if (mu_s <= 0.0f)
                return;

            double fft_points = (double)FFT_size;
            
            // Radix-2 MFLOP/s formula (https://www.fftw.org/speed/)
            double mflops = (5.0 * fft_points * log2(fft_points)) / (mu_s * scales[scale_micro_idx].scale_divisor);

            char time_buffer[STRING_LENGTH];
            char mflops_buffer[STRING_LENGTH];

            format_scaled(mu_s * scales[scale_micro_idx].scale_divisor, time_buffer, STRING_LENGTH, "s");
            format_scaled(mflops, mflops_buffer, STRING_LENGTH, "FLOP/s");

            fprintf(stdout, "%s%s%s", BAR_COLOR, line, RESET);
            fprintf(stdout, "⏱️  %sFFT per frame%s  : %s%s%s (%.3f µs)\n", 
                    BRIGHT_CYAN, RESET, BRIGHT_YELLOW, time_buffer, RESET, mu_s);
            fprintf(stdout, "⚡  %sSpeed         %s  : %s%s%s (%.3f FLOP/s)\n", 
                    BRIGHT_CYAN, RESET, BRIGHT_GREEN, mflops_buffer, RESET, mflops);
            fprintf(stdout, "%s%s%s\n\n", BAR_COLOR, line, RESET);
        }

        int compare_times(const void *a, const void *b) {
            const time_info *ta = (const time_info*)a;
            const time_info *tb = (const time_info*)b;
            
            if (tb->time_us > ta->time_us) return 1;
            if (tb->time_us < ta->time_us) return -1;
            return 0;
        }

        const char* get_gradient_color(double percentage) {
            if (percentage >= 80.0) return BRIGHT_RED;   
            if (percentage >= 60.0) return RED;
            if (percentage >= 40.0) return MAGENTA;
            if (percentage >= 25.0) return BRIGHT_YELLOW;
            if (percentage >= 15.0) return YELLOW;
            if (percentage >= 5.0) return BRIGHT_GREEN;
            if (percentage > 0.1) return GREEN;
            return BLUE;  
        }

        void print_bench_ranked(void) {
            if (benchmarks.timing_index == 0) {
                fprintf(stdout, "\nNo benchmark data available.\n");
                return;
            }

            qsort(benchmarks.timings, benchmarks.timing_index, sizeof(time_info), compare_times);

            fprintf(stdout, "%s---------------------------------------------------------\n", BRIGHT_CYAN);
            fprintf(stdout, "| %-20s | %-12s | %-7s |\n", "Function", "Exec Time", "% of total runtime");
            fprintf(stdout, "---------------------------------------------------------%s\n", RESET);

            long long max_time = benchmarks.timings[0].time_us;

            for (size_t i = 0; i < benchmarks.timing_index; i++) {
                double percentage = (double)benchmarks.timings[i].time_us * 100.0 / benchmarks.total_time;
                int filled_length = (int)(BAR_LENGTH * percentage / 100.0);

                char time_str[STRING_LENGTH];
                format_scaled(benchmarks.timings[i].time_us * scales[scale_micro_idx].scale_divisor, time_str, STRING_LENGTH, "s");

                const char *func_color = get_gradient_color((double)benchmarks.timings[i].time_us / max_time * 100.0);

                fprintf(stdout, "%s| %-20s | %12s | %6.4f%% |%s\n",
                        func_color, 
                        benchmarks.timings[i].function_name,
                        time_str,
                        percentage,
                        RESET);

                printf("%s[", BRIGHT_CYAN);
                for (int j = 0; j < filled_length; j++) printf("▰");
                for (int j = 0; j < BAR_LENGTH - filled_length; j++) printf(" ");
                printf("]%s\n", RESET);
            }

            fprintf(stdout, "%s---------------------------------------------------------\n%s", BRIGHT_CYAN, RESET);
        }

        void print_bench_json(void) {
            fprintf(stdout, ">>>{\n");
            for (size_t i = 0; i < benchmarks.timing_index; i++) {
                double percentage = (double)benchmarks.timings[i].time_us * 100.0 / benchmarks.total_time;
                fprintf(stdout, "  \"%s\": {\"time_μs\": %lld, \"percentage\": %.2f}%s\n",
                        benchmarks.timings[i].function_name,
                        (long long)benchmarks.timings[i].time_us,
                        percentage,
                        (i < benchmarks.timing_index - 1) ? "," : "");
            }
            fprintf(stdout, "}<<<\n");
        }

        void print_bench(void) {
            for (size_t i = 0; i < benchmarks.timing_index; i++) {
                fprintf(stdout, "%s:%lld\n", benchmarks.timings[i].function_name, benchmarks.timings[i].time_us);
            }
        }

        #endif // BENCH_IMPLEMENTATION

#endif // BENCH_H