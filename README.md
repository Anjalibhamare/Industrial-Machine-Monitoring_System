<div align="center">

<img width="100%" src="https://capsule-render.vercel.app/api?type=waving&color=0:0f2027,50:203a43,100:2c5364&height=180&section=header&text=Industrial%20Machine%20Monitoring%20System&fontSize=36&fontColor=00d4ff&fontAlignY=38&desc=Linux%20Systems%20Programming%20%7C%20Honours%20Project&descAlignY=58&descColor=a0cfdf&animation=fadeIn"/>

<br/>

[![Language](https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/Platform-Linux-orange?style=for-the-badge&logo=linux)](https://www.linux.org/)
[![IPC](https://img.shields.io/badge/IPC-Message%20Queue%20%2B%20Shared%20Memory-green?style=for-the-badge)]()
[![Threads](https://img.shields.io/badge/Threads-POSIX%20pthreads-purple?style=for-the-badge)]()

<br/>

> **A real-time factory monitoring system built in C on Linux. Machines generate sensor data, a monitor analyzes it, and a logger saves everything to disk — all running as separate processes communicating through IPC.**

</div>

---

## What This Project Does

This system simulates a real factory environment with 4 processes running at the same time:

| Process | File | Role |
|---------|------|------|
| **Factory Controller** | `main.c` | Starts everything, supervises all child processes |
| **Machine 1 & 2** | `machine.c` | Simulate sensors — generate production, temperature, vibration data |
| **Monitor** | `monitor.c` | Receives machine data, detects overheating or faults, raises alerts |
| **Logger** | `logger.c` | Reads processed data and writes it to a log file on disk |

---

## How the System Works

```
Factory Controller (Parent)
        |
        |── fork() ──► Machine 1  ──► sends data ──► Message Queue
        |── fork() ──► Machine 2  ──► sends data ──► Message Queue
        |── fork() ──► Monitor    ──► reads MQ   ──► writes Shared Memory
        └── fork() ──► Logger     ──► reads Shared Memory ──► machine.log
```

**Data moves in two hops:**

1. **Machine → Monitor** using a **Message Queue**
2. **Monitor → Logger** using **Shared Memory**

---

## Project Structure

```
industrial-monitor/
│
├── src/
│   ├── main.c        ← Parent controller
│   ├── machine.c     ← Machine sensor simulation
│   ├── monitor.c     ← Data analysis and alerting
│   └── logger.c      ← File logging
│
├── include/
│   └── common.h      ← Shared structs and constants
│
├── logs/
│   └── machine.log   ← Auto-generated at runtime
│
└── Makefile
```

---

## System Architecture

> See `docs/architecture.svg` for the full diagram.

<img src="docs/architecture.svg" width="100%"/>

---

## Data Flow

> See `docs/dataflow.svg` for the full diagram.

<img src="docs/dataflow.svg" width="100%"/>

---

## IPC Mechanisms Used

| Between | Mechanism | Why |
|---------|-----------|-----|
| Machine → Monitor | **Message Queue** | Multiple machines send to one monitor |
| Monitor → Logger | **Shared Memory** | Fast access to latest processed data |
| Shared Memory access | **Semaphore** | Prevents monitor and logger clashing |
| Threads inside Machine | **Mutex** | Protects shared sensor struct from race conditions |

---

## Machine Process — How Sensors Work

Each machine runs **3 threads in parallel**, one for each sensor:

| Thread | What it generates | Value range |
|--------|-------------------|-------------|
| Production thread | Units produced per second | `rand() % 100` |
| Temperature thread | Temperature in °C | `50 + rand() % 50` |
| Vibration thread | Vibration level | `rand() % 10` |

All three threads update a **shared struct** protected by a `pthread_mutex` to avoid race conditions.

Every **1 second**, the machine sends the latest readings to the Monitor via the Message Queue.

---

## Monitor — Anomaly Detection

The monitor continuously reads from the Message Queue and checks every reading:

| Condition | Threshold | Alert printed |
|-----------|-----------|---------------|
| Temperature too high | `temp > 80°C` | `ALERT: Machine X overheating` |
| Vibration too high | `vibration > 5` | `ALERT: Machine X fault` |

After checking, data is written to Shared Memory for the Logger to pick up.

---

## Logger — Writing to Disk

The logger reads from Shared Memory and appends one line per reading to `logs/machine.log`.

**Log format:**

```
timestamp | machine | production | temp | vibration | status
```

**Example entries:**

```
2024-01-15 10:23:45 | Machine1 | Prod:45 | Temp:67 | Vib:3 | OK
2024-01-15 10:23:46 | Machine2 | Prod:38 | Temp:83 | Vib:2 | ALERT
```

System calls used directly: `open()` `write()` `lseek()` `close()`

---

## Signal Handling

| Signal | How to trigger | What happens |
|--------|----------------|--------------|
| `SIGINT` | Press `Ctrl+C` | All processes stop safely, memory freed, files closed |
| `SIGUSR1` | `kill -USR1 <pid>` | Prints current status of all running processes |

When `Ctrl+C` is pressed, every process stops its loop, detaches shared memory, closes the message queue, closes open files, and prints `System shutting down safely`.

---

## Build and Run

**Build:**
```bash
make all
```

**Run:**
```bash
make run
```

**Clean:**
```bash
make clean
```

| Target | What it does |
|--------|--------------|
| `make all` | Compiles all 4 programs |
| `make run` | Starts the full system |
| `make clean` | Removes binaries and logs |
| `make test` | Runs test scenarios |

---

## Sample Output

```
======================================
  Industrial Machine Monitoring System
======================================

Factory Controller Started  |  PID: 8821
Machine 1 started  |  PID: 8822
Machine 2 started  |  PID: 8823
Monitoring system started
Logger started

Machine 1  |  Production: 45  |  Temperature: 67  |  Vibration: 3
Machine 2  |  Production: 38  |  Temperature: 72  |  Vibration: 2

Monitor → Machine 1  |  Temp: 67  ✓ OK
Monitor → Machine 2  |  Temp: 83  ⚠ ALERT: Overheating

Logger → Machine1 | Prod 45 | Temp 67 | written to log

^C
System shutting down safely
Child process terminated ×4
```

---

## Test Scenarios

### Test 1 — Normal Operation

Run the system and observe machines generating data, monitor processing it, logger writing to file with no alerts.

**Expected:** All values within range, log file grows every second, no alerts.

---

### Test 2 — Anomaly Detection

Raise temperature artificially above 80°C in the machine simulation.

**Expected:**
```
ALERT: Machine 2 overheating (Temp: 83)
```
Log file shows `ALERT` status for that entry.

---

### Test 3 — Graceful Shutdown

Press `Ctrl+C` while the system is running.

**Expected:** All 4 processes exit cleanly, no zombie processes, files closed properly.

Verify with:
```bash
ps aux | grep machine
```

---

## Linux Concepts Demonstrated

| Concept | Where it is used |
|---------|-----------------|
| `fork()` + `exec()` | Parent spawns all 4 child processes |
| `waitpid()` | Parent detects when a child exits |
| POSIX threads | 3 sensor threads per machine |
| `pthread_mutex_t` | Protects shared sensor data inside machine |
| Message Queue | Machine sends data to Monitor |
| Shared Memory | Monitor passes data to Logger |
| Semaphore | Synchronizes shared memory access |
| `open` `write` `lseek` `close` | Logger writes to disk using raw syscalls |
| `SIGINT` `SIGUSR1` | Graceful shutdown and status reporting |
| Error handling | Every system call return value is checked |

---



</div>
