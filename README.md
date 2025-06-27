# Process Scheduler Simulator

A simple C++ simulation framework for CPU schedulers with multiple algorithms, including:

- Completely Fair Scheduler (CFS)
- Round Robin (RR)
- Priority Scheduling

---

## Overview

This project models how different CPU scheduling algorithms manage runnable processes over time.  
It uses C++ polymorphism to provide a flexible design where schedulers share a common interface but implement their own logic.

---

## Features

- Base `Scheduler` class with common interface
- Derived classes for various scheduling algorithms
- Simulated process execution with virtual runtimes and priorities
- Easy to extend with new scheduling policies

---

## Build & Run

```bash
make
./cfs_scheduler
