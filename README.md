# High-Performance Exchange POC

A multi-threaded exchange system proof of concept that demonstrates modern C++ concurrency patterns and lock-free data structures for order processing.

## Architecture Overview

The system implements a high-throughput order matching engine with the following key components:

```mermaid
flowchart TD
    subgraph Clients
        C1[Client Session 1]
        C2[Client Session 2]
        C3[Client Session N]
    end

    subgraph Exchange
        A[Acceptor Application]
        Q1[Lock-free Order Queue]
        P[Order Consumer Pool]
        Q2[Notification Queue]
        E[Execution Publisher]
        B[Order Books]
    end

    C1 & C2 & C3 -->|FIX Protocol| A
    A -->|Orders| Q1
    Q1 -->|Batch Processing| P
    P -->|Matching| B
    P -->|Execution Reports| Q2
    Q2 -->|Notifications| E
    E -->|Updates| C1 & C2 & C3
```

## Key Features

- **Multi-threaded Architecture**: Thread pool for parallel order processing
- **Lock-free Data Structures**: Concurrent queues for order processing
- **FIX Protocol Support**: FIX 4.2 protocol implementation
- **Efficient Order Matching**: d-ary heaps for priority queue implementation
- **Batch Processing**: Optimized order and notification processing
- **Market Data Updates**: Real-time market data dissemination

## Technical Implementation

### Core Components

1. **Acceptor Application**

   - FIX protocol client connection handling
   - Session lifecycle management
   - Order routing to processing pipeline

2. **Order Consumer Pool**

   - Parallel order processing
   - Batch processing implementation
   - Matching logic and trade execution

3. **Order Book**

   - Price-time priority matching
   - d-ary heap-based order management
   - Market and limit order handling

4. **Execution Publisher**
   - Client execution report publishing
   - Market data update management
   - Notification batching

## Dependencies

- C++23
- QuickFIX (FIX protocol implementation)
- Boost (system, thread)
- BS Thread Pool
- ConcurrentQueue (lock-free queue implementation)
- Unordered Dense Map (high-performance hash map)
- nlohmann/json (JSON parsing)

## Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

## Project Structure

```
.
├── include/                 # Header files
├── src/                     # Source files
└── CMakeLists.txt         # Build configuration
```

## Further Required Improvements

- Add persistence layer for order book state
- Implement more sophisticated matching algorithms
- Add risk management features
- Enhance monitoring and metrics collection
- Add more comprehensive testing suite
