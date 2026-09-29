*This project has been created as part of the 42 curriculum by elbarry.*

# Codexion

## Description

Codexion is a C concurrency simulation in which coders share a circular set of USB dongles. Each coder is represented by a POSIX thread and must acquire the two adjacent dongles before compiling. After compiling, the coder releases both dongles, debugs, refactors, and then requests the dongles again.

The simulation stops when either every coder has reached the required number of compilations or a coder misses its burnout deadline.

The project focuses on thread synchronization, fair resource arbitration, cooldown periods, priority queues, precise monitoring, and serialized output.

## Instructions

### Compilation

Requirements:

- C compiler supporting POSIX threads.
- `pthread` support.

Build with:

```sh
make
```

The project is compiled with `-Wall -Wextra -Werror -pthread`.

Clean build files:

```sh
make clean
```

Remove all generated files:

```sh
make fclean
```

Rebuild:

```sh
make re
```

### Execution

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are mandatory. Durations are expressed in milliseconds. `scheduler` must be exactly `fifo` or `edf`.

Example:

```sh
./codexion 4 800 200 200 200 3 10 edf
```

## Scheduling model

### FIFO

Requests are ordered by their arrival sequence. The request that entered the scheduler first is considered first.

### EDF

Requests are ordered by the burnout deadline:

```text
last_compile_start + time_to_burnout
```

If two deadlines are equal, the request sequence number is used as a deterministic tie-breaker.

### Atomic pair acquisition

A coder never holds one dongle while waiting for the other. The scheduler removes a request from the heap, then acquires both required dongle mutexes in a fixed index order and reserves both dongles before waking the coder. If the reservation is no longer possible, the request is requeued. This avoids the classic circular-wait pattern of the dining-philosophers problem.

## Blocking cases handled

### Deadlock prevention and Coffman's conditions

The implementation avoids the hold-and-wait condition by granting a coder its complete pair of dongles atomically. A coder therefore cannot keep one dongle while waiting for its neighbour's dongle. The two dongle mutexes are acquired in a deterministic order.

### Starvation prevention

The scheduler uses a custom binary heap and a deterministic ordering. FIFO preserves request arrival order. EDF orders by the earliest burnout deadline and uses arrival order as a tie-breaker. A coder remains represented by one pending request until the scheduler grants the pair.

### Cooldown handling

Every dongle stores an `available_at` timestamp. After release, the scheduler cannot grant the dongle before that timestamp. The scheduler sleeps on a condition variable and wakes when a release, a new request, or a relevant cooldown timeout occurs.

### Precise burnout detection

A dedicated monitor thread checks each active coder's last compile start against its burnout deadline. The monitor wakes frequently enough to keep the burnout message within the subject's required 10 ms tolerance on a normally scheduled system.

### Log serialization

All output is protected by a dedicated output mutex. The two `has taken a dongle` lines and the following `is compiling` line are emitted as one serialized block, so another thread cannot insert a log line between them.

## Thread synchronization mechanisms

### `pthread_mutex_t`

The program uses mutexes for:

- global simulation state;
- serialized output;
- the request heap and scheduler condition;
- each dongle's owner and cooldown state.

For example, a dongle's owner and `available_at` fields are only read or modified while its mutex is held.

### `pthread_cond_t`

Condition variables are used to avoid busy-waiting:

- one condition variable wakes the scheduler when a request or dongle release may change scheduling availability;
- one condition variable per coder wakes a coder when its pair has been granted;
- one condition variable wakes the monitor when simulation state changes.

Timed waits are used for cooldown and burnout deadlines.

### Custom event implementation

The request heap is the project's custom event queue. Each pending request stores its coder, arrival sequence and EDF deadline. The heap is protected by the queue mutex, so concurrent coder threads cannot corrupt its structure. The scheduler and coder condition variables provide the event/wakeup mechanism without busy-waiting.

## Race-condition examples

A coder cannot simultaneously observe a dongle as free while another coder owns it because the owner check and owner assignment are performed while the dongle mutex is held.

A request cannot be removed by two scheduler operations because the request heap is protected by the queue mutex.

A coder only starts compiling after the scheduler has atomically reserved both required dongles for it.

The monitor reads the last compile timestamp while coder state changes are coordinated by the simulation's synchronization rules, and the stop flag is protected by the state mutex.

## Memory management

All heap allocations are released before program termination. Thread, condition-variable, mutex, heap, coder and dongle resources are explicitly destroyed.

## Resources

Classic references:

- POSIX Threads (`pthread_create`, `pthread_join`, mutexes and condition variables): POSIX / The Open Group documentation.
- `gettimeofday` and `clock_gettime`: Linux/POSIX time documentation.
- The Dining Philosophers problem and Coffman's deadlock conditions for concurrency fundamentals.
- Binary heaps / priority queues for deterministic request arbitration.

### AI usage

Artificial intelligence tools were used during the development of this project to assist with:

* brainstorming implementation ideas;
* improving documentation and README structure;
* reviewing code quality;
* identifying potential bugs and edge cases;
* debugging and understanding error messages.

All architectural decisions, implementation, debugging, testing and final validation were performed and reviewed by the project author.

## Evaluation-oriented checklist

- [x] C implementation.
- [x] One thread per coder.
- [x] One dongle between each pair of coders.
- [x] Single-coder case cannot acquire two dongles and burns out.
- [x] Mutex-protected dongle state.
- [x] Mandatory dongle cooldown.
- [x] FIFO and EDF arbitration.
- [x] Deterministic EDF tie-breaker.
- [x] Separate monitor thread.
- [x] Serialized logging.
- [x] Custom binary heap.
- [x] Simulation stop conditions.
- [x] Required Makefile targets.
- [x] No libft dependency.
