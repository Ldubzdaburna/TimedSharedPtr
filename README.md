# TimedSharedPtr – Final Exam Project

**Author:** Lyles Williams  
**Course:** CSE-3150 – C++ Essentials  
**Lab:** Final Exam  

##  Overview
This project implements a custom smart pointer called `TimedSharedPtr<T>`. It works like `shared_ptr`, but with a timer. After a certain number of milliseconds, the pointer "expires," and calls to `get()` will return `nullptr`.

##  How It Works
- When you create a `TimedSharedPtr`, it sets up a control block.
- This block keeps track of:
  - The actual pointer
  - A reference counter
  - The start time
  - The time when the pointer should expire
- Every time you call `get()`, it checks how much time has passed. If the time is up, it returns `nullptr`.

##  Code Structure
- **ControlTimedSharedPtr** – Holds the pointer, the reference count, and the expiration details.
- **TimedSharedPtr<T>** – The smart pointer class users interact with. Handles copying and expiration.
- `use_count()` – Tells you how many pointers are sharing the resource.
- `get()` – Returns the pointer if still valid, or `nullptr` if expired.

##  Unit Testing
Unit tests are written using the [Doctest](https://github.com/doctest/doctest) framework. Tests check:
- That the pointer is available before expiration
- That it returns `nullptr` after the timer runs out
- That the reference count behaves correctly

Run tests with:
```sh
g++ -std=c++17 TimeSharedPtr_test.cpp -o test && ./test
```

##  Example
```cpp
TimedSharedPtr<Node> myNode(new Node(7), 100); // expires in 100 ms
this_thread::sleep_for(chrono::milliseconds(50));
cout << myNode.get(); // Still valid

this_thread::sleep_for(chrono::milliseconds(75));
cout << myNode.get(); // Now expired – returns nullptr
```

##  AI Usage Disclosure
A small part of the example in `main.cpp` was generated using ChatGPT to help format timing and output. All implementation logic was written and understood by the author.

##  Files
- `TimeSharedPtr.h` – Header with full implementation
- `main.cpp` – Sample use case
- `TimeSharedPtr_test.cpp` – Doctest unit tests
- `doctest.h` – Testing framework (single-header)
