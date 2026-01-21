*This project has been created as part of the 42 curriculum by ruisilva*

# Codexion

## Description
**Codexion** is a concurrent programming project from the 42 curriculum that reimagines the classic **Dining Philosophers** problem. In this multithreaded simulation, **coders** sitting around a virtual table must share a limited set of **dongles** to get their work done.

The challenge is one of resource management and synchronization. To **compile** code, a coder requires exclusive access to two specific dongles. Once compilation is complete, they release the resources and proceed to **debug** and **refactor**. The system must carefully orchestrate these threads to prevent deadlocks and starvation; if a coder waits too long for dongles, they suffer **burnout**, and the simulation fails.

## Instructions
### 1. Compilation
To compile the `codexion` program:

```bash
make
```

### 2. Execution
Run the simulation with the following arguments: `./codexion [number_of_coders] [time_to_burnout] [time_to_compile] [time_to_debug] [time_to_refactor] [compiles_required] [dongle_cooldown] [scheduler]`

| Argument | Description |
| :---- | :---- |
| `number_of_coders` | The number of coders (threads) and dongles. |
| `time_to_burnout` | Time (in ms) a coder can go without compiling before burning out. |
| `time_to_compile` | Duration (in ms) of the compilation process (requires 2 dongles). |
| `time_to_debug` | Duration (in ms) spent debugging after compiling. |
| `time_to_refactor` | Duration (in ms) spent refactoring after debugging. |
| `compiles_required` | Number of times each coder must compile to finish the simulation. |
| `dongle_cooldown` | Time (in ms) a dongle remains unavailable after being used. |
| `scheduler` | The scheduling algorithm to use: fifo or edf. |

**Example:**

```bash
./codexion 5 800 200 200 200 5 10 fifo
```

## Blocking cases handled
This simulation robustly addresses standard concurrency issues to ensure stability and correctness:

- **Deadlock Prevention (Coffman's Conditions)**: To prevent circular dependencies where every coder holds one dongle and waits for another, the solution implements a **Resource Hierarchy** strategy. By enforcing a strict locking order (coders always attempt to lock the dongle with the higher memory address first), the "Circular Wait" condition is mathematically impossible.

- **Starvation Prevention**: A custom **FIFO (First-In-First-Out) Queue** is implemented to manage dongle requests. When dongles are busy, coders are added to a waiting list. This ensures fairness: no coder can be indefinitely bypassed by faster threads, preventing starvation even under high load.

- **Precise Burnout Detection**: A dedicated **Monitor Thread** continuously scans the state of all coders. It utilizes a mutex-protected heartbeat mechanism (`last_compile_time`) to detect inactivity. If a coder fails to compile within the `time_to_burnout` window (adjusted for execution delay), the monitor instantly flags the simulation as "burned out" and halts execution.

- **Cooldown Handling**: To simulate resource unavailability, dongles have a mandatory cooldown period. The system calculates a `next_available_time` for each dongle upon release. Threads attempting to access a dongle during this window are forced to wait, effectively simulating hardware constraints.

- **Log Serialization**: All output to the console is guarded by a shared `print_lock` mutex. This prevents race conditions in the standard output (scrambled or interleaved messages) when multiple threads attempt to print status updates simultaneously.

## Thread synchronization mechanisms
The project utilizes strict POSIX threading primitives to coordinate access to shared resources:

### Mutexes 
`pthread_mutex_t`

Mutexes are the primary defense against race conditions, ensuring mutually exclusive access to critical data:

- **Dongle Locks**: Each dongle has a dedicated mutex. A coder must successfully acquire both locks (left and right) to enter the critical "compiling" section.

- **Simulation Lock** (`sim_lock`): A global mutex protects shared simulation state, including the FIFO queue and the `is_simulation_over` flag. This prevents "torn reads" where the Monitor might read a value while a Coder is partially updating it.

- **Print Lock** (`print_lock`): A mutex specifically designated for I/O operations ensures atomic log entries.

### Condition Variables
(`pthread_cond_t`)

To maximize CPU efficiency, the simulation uses condition variables instead of busy-waiting loops:

- **Efficient Waiting**: When dongles are unavailable, a coder enters a sleep state using `pthread_cond_wait` (or `timedwait`). This releases the CPU for other threads (like the Monitor) to run.

- **Broadcasting Events**: When a coder finishes refactoring and returns their dongles, they trigger `pthread_cond_broadcast`. This signal wakes up all waiting threads in the queue, allowing them to re-check resource availability.

### Monitor-Coder Communication

Communication is achieved via shared memory protected by the `sim_lock`. The Monitor passively observes the `last_compile_time` of each coder. Spurious wakeups (threads waking up without a signal) are handled by wrapping the wait condition in a `while` loop. A coder waking up immediately re-validates that dongles are actually free before proceeding; if not, they return to sleep, preserving data integrity.

## Resources
Since this project relies heavily on the POSIX standard, the following documentation was referenced:

- **POSIX Threads (pthreads) Specification**: [The Open Group Base Specifications Issue 7](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html) - For details on pthread_mutex, pthread_cond, and thread creation.

- **The Dining Philosophers Problem**: [Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem) - For the theoretical background of the resource sharing problem.

- **Valgrind User Manual**: [Helgrind: a thread error detector](https://valgrind.org/docs/manual/hg-manual.html) - For interpreting data race reports.

### AI Usage
AI assistance was utilized in this project for:

- **Conceptual Tutoring**: acting as a Socratic tutor to clarify doubts regarding core concurrent programming concepts. This included explaining the theoretical behavior of threads, the mechanics of race conditions, and the differences between busy-waiting versus sleeping.

- **Debugging Analysis**: Analyzing complex logs from Valgrind and Helgrind to identify the root causes of race conditions and distinguish them from false positives.

- **Edge Case Generation**: Suggesting rigorous stress tests (e.g., 200 threads, 0 durations) to verify system stability under load.

- **Documentation Drafting**: Assisting in structuring and refining the README file to ensure clarity, coherence, and adherence to project submission requirements.