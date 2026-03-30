# 🏭 Industrial Machine Monitoring System using Linux System Calls

---

## 📌 1. Introduction

Modern industries rely heavily on automated systems to monitor machine performance and detect faults in real time.  
This project simulates an **Industrial Machine Monitoring System** using Linux system programming concepts.

The system consists of:
- Multiple machines generating sensor data  
- A monitoring unit analyzing the data  
- A logging system storing it  

This project demonstrates how real industrial systems (like SCADA) work internally.

---

## 🎯 2. Objectives

- Understand Linux system calls  
- Implement multi-process architecture  
- Simulate real-time machine data  
- Use threads for parallel processing  
- Demonstrate file handling in Linux  
- Design a modular and scalable monitoring system  

---

## 🏭 3. System Overview

The system simulates a factory where:

- Multiple machines generate data  
- A monitor analyzes data  
- A logger stores data  

---

## 🔁 Overall Working Flow

## 🔁 System Flow

**Main Controller**  
⬇️  
**Creates Processes**  
⬇️  
**Machine Processes (Threads generate data)**  
⬇️  
**Monitor Process (Analyzes data)**  
⬇️  
**Logger Process (Writes data to file)**

## 🏗️ 4. System Architecture

### 🔹 Components

### 1️⃣ Main Controller (`kpit.c`)
- Controls entire system  
- Creates processes  

### 2️⃣ Machine Processes (`machine.c`)
- Simulate machines  
- Generate sensor data using threads  

### 3️⃣ Monitor Process (`monitor.c`)
- Receives and processes data  
- Detects abnormal conditions  

### 4️⃣ Logger Process (`logger.c`)
- Writes data to log file  

---

## ⚙️ 5. Detailed Working of Each Module

---

### 🔹 5.1 Main Controller (`kpit.c`)

#### Responsibilities:
- Starts system execution  
- Creates child processes using `fork()`  
- Executes programs using `exec()`  
- Manages processes using `waitpid()`  

#### Working:
- Program starts  
- Calls `fork()` multiple times  
- Each child runs:
  - machine  
  - monitor  
  - logger  
- Parent waits for child processes  

---

### 🔹 5.2 Machine Module (`machine.c`)

#### Purpose:
Simulates an industrial machine using **multithreading**

---
#### Step-by-step Working:

### 1. Read Machine ID
- Uses `argv[1]`





## 🧠 Thread Details

| Thread | Function |
|--------|---------|
| Production Thread | Generates production count |
| Temperature Thread | Generates temperature |
| Vibration Thread | Generates vibration |

---

## 📊 Output Example

Machine 1 → Temp: 65, Prod: 40, Vib: 3


## 🧠 Monitor Module (`monitor.c`)

### 🎯 Purpose
Stores and processes incoming machine data and acts as the **central processing unit**

---

## 🔄 Working Flow


**Start Monitoring**
      ↓
**Receive Data (Message Queue - Planned)**
      ↓
**Analyze Values**
      ↓
**Check Conditions**
      ↓
**Generate Alerts**

### ⚠️ Condition Checking

-Temperature > 80 → Alert
-Vibration > threshold → Warning

### 📊 Output Example
-Machine 2 overheating

### 🔄 Working Flow
**Create Log File**
      ↓
**Write Data**
      ↓
**Move File Pointer (lseek)**
      ↓
**Continuous Logging**
      ↓
**Handle Signal (SIGINT)**
      ↓
**Close File**

### ⚙️ File Operations
Operation	Function
| Operation | Function |
|--------|---------|
| Create file| open() |
| Write data | write() |
| Move pointer | lseek()|

⚠️ Signal Handling
SIGINT (Ctrl + C)

### ⚠️ Signal Handling

**Actions:**
- Close file  
- Exit safely  

---

## 🧠 Technologies Used

---

### 🔹 Process Control

| Function     | Purpose            |
|-------------|--------------------|
| `fork()`    | Create processes   |
| `exec()`    | Execute programs   |
| `waitpid()` | Manage processes   |

---

### 🔹 Threads
- Parallel execution  
- Sensor simulation  

---

### 🔹 Mutex
- Prevents data corruption  
- Ensures safe access  

---

### 🔹 File Handling

| Function | Purpose      |
|----------|-------------|
| `open()` | Create file |
| `write()`| Write data  |
| `lseek()`| Move pointer|

---

### 🔹 Signals

| Signal | Purpose           |
|--------|------------------|
| SIGINT | Graceful shutdown|

---

### 🔹 IPC (Planned)

- Message Queue → Machine → Monitor communication  
- Shared Memory → Monitor → Logger communication  
## 🔄 Data Flow
**Machine (Threads)**
        ↓
**Generate Data**
        ↓
**Message Queue (planned)**
        ↓
**Monitor**
        ↓
**Shared Memory (planned)**
        ↓
**Logger**
        ↓
**Log File**

## ⚙️ File Operations

| Operation     | Function  |
|--------------|----------|
| Create file  | `open()` |
| Write data   | `write()`|
| Move pointer | `lseek()`|

---

## ⚠️ Signal Handling

SIGINT (Ctrl + C)
