#pragma once

#include <atomic>

inline std::atomic<bool> global_processing_state{false};
inline std::atomic<bool> execution_publisher_running{false};

constexpr int heap_arity = 4;
constexpr bool heap_mutability = false;
constexpr size_t processor_thread_count = 16;
constexpr size_t processed_batch_size = 128;

constexpr size_t execution_publisher_thread_count = 4;
constexpr size_t execution_publisher_batch_size = 128;