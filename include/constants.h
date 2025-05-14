#pragma once

#include <atomic>
constexpr std::atomic<bool> global_processing_state{false};
constexpr int heap_arity = 4;
constexpr bool heap_mutability = false;
constexpr size_t processor_thread_count = 16;
constexpr size_t processed_batch_size = 128;