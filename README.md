# 🚀 bench.h - Single Header Benchmark Library

A lightweight, easy-to-use C benchmarking library with SI unit scaling, colorful output, and FFT performance analysis.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C](https://img.shields.io/badge/C-99%2B-green.svg)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS-lightgrey.svg)

## ✨ Features

- 🎯 **Drop-in simplicity**: Single header file, no dependencies
- ⚡ **High-precision timing**: Microsecond accuracy using `CLOCK_MONOTONIC`
- 📊 **Multiple output formats**: Raw, JSON, and ranked visualization
- 🌈 **Colorful terminal output**: ANSI colors with gradient-based performance ranking
- 📏 **SI unit scaling**: Automatic scaling from nanoseconds to teraseconds
- 🔢 **FFT performance analysis**: Built-in MFLOP/s calculation for FFT operations
- 📝 **Rich logging**: INFO/WARN/ERROR macros with source location metadata
- 🎨 **Visual progress bars**: Unicode-based performance comparison charts

## 🏁 Quick Start

### Installation
Simply download `bench.h` and include it in your project:

```c
#define BENCH_IMPLEMENTATION
#include "bench.h"

int main() {
    benchmark_init();
    
    START_TIMING();
    // Your code here
    END_TIMING("my_function");
    
    print_bench_ranked();
    return 0;
}
```

### Compilation
```bash
gcc -o myprogram myprogram.c -lm -lrt
```

## 📖 Basic Usage

### Simple Timing
```c
#define BENCH_IMPLEMENTATION
#include "bench.h"

void my_algorithm() {
    // Some computation
    for (int i = 0; i < 1000000; i++) {
        volatile double x = sin(i) * cos(i);
    }
}

int main() {
    benchmark_init();
    
    START_TIMING();
    my_algorithm();
    END_TIMING("my_algorithm");
    
    print_bench_ranked();
    return 0;
}
```

### Multiple Functions
```c
benchmark_init();

START_TIMING();
quick_sort(data, size);
END_TIMING("quicksort");

START_TIMING();
merge_sort(data, size);
END_TIMING("mergesort");

START_TIMING();
heap_sort(data, size);
END_TIMING("heapsort");

print_bench_ranked();
```

## 🎯 Advanced Features

### FFT Performance Analysis
```c
START_TIMING();
fft_radix2(samples, 1024);
END_TIMING("fft_1024");

// Get timing and calculate MFLOP/s
benchmark_t* bench = get_bench_instance();
double time_us = bench->timings[bench->timing_index - 1].time_us;
FFT_bench(time_us, 1024);
```

**Output:**
```
▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰
⏱️  FFT per frame  :   2.341 ms (2341.000 µs)
⚡  Speed          :  21.847 MFLOP/s (21847000.000 FLOP/s)
▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰
```

### Output Formats

#### Raw Output
```c
print_bench();
```
```
quicksort:1234
mergesort:2341
heapsort:1876
```

#### JSON Output
```c
print_bench_json();
```
```json
>>>{
  "quicksort": {"time_μs": 1234, "percentage": 22.15},
  "mergesort": {"time_μs": 2341, "percentage": 42.03},
  "heapsort": {"time_μs": 1876, "percentage": 33.67}
}<<<
```

#### Ranked Visualization
```c
print_bench_ranked();
```
```
---------------------------------------------------------
| Function             | Exec Time    | % of total runtime |
---------------------------------------------------------
| mergesort            |    2.341 ms  |  42.03% |
[████████▰▰▰▰▰▰▰▰▰▰▰▰]
| heapsort             |    1.876 ms  |  33.67% |
[██████▰▰▰▰▰▰▰▰▰▰▰▰▰▰]
| quicksort            |    1.234 ms  |  22.15% |
[████▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰]
---------------------------------------------------------
```

### Logging System
```c
LOG("Processing %zu samples at %d Hz", num_samples, sample_rate);
WARN("Buffer nearly full: %d/%d", current, max_size);
ERROR("Memory allocation failed for %zu bytes", buffer_size);
```

**Output:**
```
[INFO] [file: main.c | line: 42 | func: process_audio] Processing 1024 samples at 44100 Hz
[WARN] [file: main.c | line: 58 | func: fill_buffer] Buffer nearly full: 950/1024
[ERROR] [file: main.c | line: 73 | func: allocate_memory] Memory allocation failed for 8388608 bytes
```

## 🔧 API Reference

### Core Functions
| Function | Description |
|----------|-------------|
| `benchmark_init()` | Initialize the benchmark system |
| `START_TIMING()` | Begin timing a code block |
| `END_TIMING(name)` | End timing and record under `name` |
| `print_bench()` | Print raw timing data |
| `print_bench_json()` | Print JSON formatted results |
| `print_bench_ranked()` | Print ranked visualization |

### FFT Analysis
| Function | Description |
|----------|-------------|
| `FFT_bench(time_us, size)` | Calculate and display FFT MFLOP/s |

### Utility Functions
| Function | Description |
|----------|-------------|
| `get_time_us()` | Get current time in microseconds |
| `format_scaled(val, buf, size, unit)` | Format value with SI scaling |
| `get_scale(value)` | Get appropriate SI scale for value |

### Logging Macros
| Macro | Description |
|-------|-------------|
| `LOG(...)` | Print info message with metadata |
| `WARN(...)` | Print warning message with metadata |
| `ERROR(...)` | Print error message with metadata |

## 🎨 SI Unit Scaling

The library automatically scales values to the most readable SI unit:

| Value | Displayed As |
|-------|-------------|
| `0.000000123` | `123.000 ns` |
| `0.000456` | `456.000 µs` |
| `0.123` | `123.000 ms` |
| `1.5` | `1.500 s` |
| `1500.0` | `1.500 ks` |
| `1500000.0` | `1.500 Ms` |

## 🔬 FFT Performance Analysis

The library includes specialized FFT benchmarking using the standard radix-2 complexity formula:

```
MFLOP/s = (5.0 × N × log₂(N)) / (time_in_seconds)
```

Based on [FFTW benchmarking methodology](https://www.fftw.org/speed/).

## ⚙️ Configuration

### Compile-time Constants
```c
#define MAX_FUNS_TO_BENCH    600    // Maximum functions to track
#define MAX_FUNS_NAME_LENGTH 100    // Maximum function name length  
#define BAR_LENGTH           20     // Progress bar width
#define STRING_LENGTH        32     // Internal buffer size
```

### Color Customization
All colors can be customized by modifying the ANSI color defines:
```c
#define BRIGHT_RED      "\x1b[91m"
#define BRIGHT_GREEN    "\x1b[92m"
#define BRIGHT_YELLOW   "\x1b[93m"
// ... etc
```

## 🧪 Example Programs

### Basic Benchmarking
```c
#define BENCH_IMPLEMENTATION
#include "bench.h"
#include <math.h>

void cpu_intensive() {
    volatile double sum = 0;
    for (int i = 0; i < 1000000; i++) {
        sum += sin(i) * cos(i);
    }
}

void memory_intensive() {
    const size_t size = 1000000;
    int *data = malloc(size * sizeof(int));
    if (data) {
        for (size_t i = 0; i < size; i++) {
            data[i] = rand();
        }
        free(data);
    }
}

int main() {
    benchmark_init();
    
    START_TIMING();
    cpu_intensive();
    END_TIMING("cpu_intensive");
    
    START_TIMING();
    memory_intensive();
    END_TIMING("memory_intensive");
    
    print_bench_ranked();
    return 0;
}
```

### Algorithm Comparison
```c
#define BENCH_IMPLEMENTATION
#include "bench.h"

// Compare different sorting algorithms
int main() {
    benchmark_init();
    
    int data[10000];
    
    // Test bubble sort
    generate_random_data(data, 10000);
    START_TIMING();
    bubble_sort(data, 10000);
    END_TIMING("bubble_sort");
    
    // Test quick sort
    generate_random_data(data, 10000);
    START_TIMING();
    quick_sort(data, 0, 9999);
    END_TIMING("quick_sort");
    
    // Test merge sort
    generate_random_data(data, 10000);
    START_TIMING();
    merge_sort(data, 0, 9999);
    END_TIMING("merge_sort");
    
    printf("\n🏆 Sorting Algorithm Performance Comparison:\n");
    print_bench_ranked();
    
    return 0;
}
```

## 🔧 Requirements

- **Compiler**: C99 or later
- **Platform**: Linux, macOS (requires POSIX `clock_gettime`)
- **Libraries**: `-lm` (math library), `-lrt` (real-time library on some systems)

## 📊 Sample Output

```
═══════════════════════════════════════════════════════════════
                    BENCHMARK RESULTS                      
═══════════════════════════════════════════════════════════════

---------------------------------------------------------
| Function             | Exec Time    | % of total runtime |
---------------------------------------------------------
| slow_operation       |    5.234 ms  |  45.67% |
[█████████▰▰▰▰▰▰▰▰▰▰▰]
| memory_intensive     |    3.456 ms  |  30.12% |
[██████▰▰▰▰▰▰▰▰▰▰▰▰▰▰]
| medium_operation     |    1.789 ms  |  15.61% |
[███▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰]
| fast_operation       |  234.567 µs  |   8.60% |
[█▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰]
---------------------------------------------------------
```

## 🤝 Contributing

Contributions are welcome! Please feel free to submit issues, feature requests, or pull requests.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

```
Copyright © 2025 Devadut S Balan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
```

## 🎯 Use Cases

- **Algorithm Performance Comparison**: Compare different implementations
- **Optimization Validation**: Measure performance improvements
- **Profiling Hot Spots**: Identify performance bottlenecks
- **FFT Performance Analysis**: Specialized DSP benchmarking
- **Development Debugging**: Quick timing checks during development
- **Performance Regression Testing**: Track performance over time

## 🚀 Why bench.h?

- **Zero Dependencies**: Just standard C library
- **Single File**: Easy to integrate and distribute
- **Professional Output**: Publication-ready performance charts
- **FFT Specialized**: Built for DSP and scientific computing
- **SI Scaling**: Human-readable timing across all scales
- **Cross-Platform**: Works on Linux and macOS
- **Lightweight**: Minimal overhead on your application

---

**Happy Benchmarking! 🎯⚡**