# C++ STL Priority Queue Implementation

This project implements a custom version of the C++ Standard Template Library (STL) priority queue container. The implementation provides exactly the same interface and performance characteristics as `std::priority_queue`.

## Overview

The custom priority queue is a container adaptor that provides constant time lookup of the largest element, logarithmic insertion, and logarithmic removal. Key features include:

- Template-based implementation supporting any data type
- Customizable comparison function for priority ordering
- Customizable underlying container (defaults to `custom::vector<T>`)
- Constant time access to the top (highest priority) element
- Logarithmic time insertion and removal operations
- Size and empty status methods
- Standard heap-based implementation

## Class Structure

### `custom::priority_queue<T, Container, Compare>`

The main priority queue class template with three parameters:

- T: Type of elements
- Container: Type of underlying container (default `custom::vector<T>`)
- Compare: Comparison function (default `std::less<T>`)

Key methods:

- `top()`: Access the highest priority element
- `push()`: Insert element into the queue with proper priority
- `pop()`: Remove the highest priority element
- `size()`, `empty()`: Container info
- `heapify()`: Internal method to ensure heap property

## Usage Example

```cpp
#include "priority_queue.h"

// Create priority queue of integers
custom::priority_queue<int> pq;

// Add elements
pq.push(3);
pq.push(1);
pq.push(4);
pq.push(2);

// Access top element (4)
int highest = pq.top();

// Remove elements in priority order
while (!pq.empty()) {
    std::cout << pq.top() << " ";
    pq.pop();
}
// Output: 4 3 2 1
```

## Testing

The implementation includes comprehensive unit tests in `testPriorityQueue.h` that verify:

- Construction and destruction behavior
- Element access operations
- Push and pop functionality
- Heap property maintenance
- Memory management
- Edge cases

Run tests by building in debug mode with the DEBUG flag defined.

## Files

- `priority_queue.h`: Main priority queue implementation
- `vector.h`: Underlying container implementation
- `testPriorityQueue.h`: Unit tests
- `spy.h`: Helper class for testing
- `unitTest.h`: Unit testing framework

## Building

The project includes Visual Studio solution files for building on Windows. Open the solution file and build using Visual Studio 2019 or later.

## Notes

- This is an educational implementation focused on demonstrating STL container adaptor concepts
- The implementation aims to match `std::priority_queue`'s interface and performance
- The priority queue uses a binary heap data structure internally
- By default, it creates a max heap where the largest element is at the top

### Disclaimer

This README was initially generated using an AI Language Model (Claude 3.7 Sonnet Thinking) and subsequently edited by a human for accuracy and completeness. While the content accurately describes the codebase, the writing structure and initial draft were AI-assisted. The actual code implementation and testing were completed by human developers.

## License

This code is provided for educational purposes. See included license file for terms of use. [TODO: add LICENSE]

## Authors

Nathan Bird  
[nathanbirdka@gmail.com](mailto:nathanbirdka@gmail.com)

Brock Hoskins  
[](.)
