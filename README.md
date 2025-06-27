# CFS Scheduler Simulator

A simple C++ simulation of the **Completely Fair Scheduler (CFS)** used in modern Linux kernels.  
This project models how CFS manages processes using `vruntime` and weights to achieve fair CPU time distribution.

## 🧠 What is CFS?

CFS is the default Linux process scheduler. It aims to share CPU time **fairly** between runnable processes, based on their weights (or "niceness").  
Instead of using fixed time slices, it maintains a **virtual runtime (vruntime)** for each process and always schedules the one with the lowest vruntime.

---

## 🎯 Features

- Simulated process creation and execution
- Custom weights (priorities) for each process
- Accurate vruntime tracking
- Red-black tree behavior using `std::set`
- Event logging to track scheduling decisions

---

## 📁 Project Structure

```
cfs_simulator/
├── src/
│   ├── main.cpp         # Simulation loop
│   ├── scheduler.cpp/h  # Scheduler logic
│   ├── process.cpp/h    # Process representation
│   └── utils.cpp/h      # (Optional) Helpers and logging
├── logs/                # Event logs (optional)
├── Makefile             # Build instructions
└── README.md            # You're reading it
```

---

## 🛠️ Build & Run

### Requirements
- C++17 or later
- Linux or WSL (for easy building)

### Build
```bash
make
```

### Run
```bash
./cfs_simulator
```

---

## 📌 How It Works

Each process is inserted into a red-black tree (via `std::set`) sorted by `vruntime`.  
The scheduler:
1. Picks the process with the lowest `vruntime`
2. Simulates running it for a time slice (based on weight)
3. Updates `vruntime`
4. Re-inserts it into the tree

---

## 📝 Example Output

```
[  0ms] Created process P1 (weight=1024)
[ 10ms] Scheduled P1 (vruntime=0)
[ 30ms] Scheduled P2 (vruntime=5)
[ 55ms] P1 back on CPU (vruntime=20)
```

---

## ✍️ Future Features

- Logging to file in JSON
- Visual timeline (CLI or web)
- Support for I/O-bound processes
- Interactive shell to create/kill processes live

---

## 📚 Learning Goals

- Understand how modern CPU schedulers work
- Practice with C++ classes, STL containers, and simulation design
- Explore OS-level concepts through code

---

## 📄 License

MIT License – feel free to use or modify this for learning or teaching.
