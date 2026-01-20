# 🚀 ESP32 DevOps Analyzer Showcase

[![Software Factory ESP32](https://github.com/elross913/esp32-devops-analyzer/actions/workflows/main.yml/badge.svg)](https://github.com/elross913/esp32-devops-analyzer/actions/workflows/main.yml)

This project is a technical demonstration of an **Embedded Software Factory**. It implements a robust ESP32 firmware using **ESP-IDF** and **FreeRTOS**, fully integrated into a **CI/CD** pipeline.

The goal is to generate signals across various protocols (UART, I2C, SPI) to validate logic analyzer usage while ensuring code quality through automation.

## 🛠️ Software Architecture
The firmware is built on the **FreeRTOS** kernel with a strict separation of concerns using asynchronous tasks:

* **UART Task** (Priority 5): Periodic monitoring log transmission.
* **I2C Task** (Priority 4): Traffic simulation to a slave sensor (0x42).
* **SPI Task** (Priority 6): High-priority frame generation for timing tests.

## 🏗️ Software Factory (DevOps)
The project integrates industry best practices to secure the development lifecycle:

* **Static Analysis:** Integrated `cppcheck` and `clang-tidy` via PlatformIO to detect bugs before execution.
* **CI/CD (GitHub Actions):** Every push triggers a build and analysis. If a "High" severity quality issue is detected, the build fails.
* **Docker:** A reproducible build environment is provided to compile the project without polluting the local machine.
* **Artifacts:** The final binary (`firmware.bin`) is automatically generated and stored in GitHub Actions.

## 🔌 Hardware Setup (Logic Analyzer)
To test your logic analyzer, connect your probes to the following ESP32 pins:

| Protocol | Signal | GPIO Pin | Configuration |
| :--- | :--- | :--- | :--- |
| **UART 2** | TX | 17 | 115200 baud, 8N1 |
| **I2C 0** | SDA / SCL | 21 / 22 | Master, 100kHz |
| **SPI 2** | MOSI / CLK / CS | 23 / 18 / 5 | Mode 0, 1MHz |



## 🚀 Installation & Build

### Using Docker (Recommended)
To compile the project without installing any dependencies:
```bash
docker build -t esp32-factory .
docker run --rm -v $(pwd):/app -e PIO_BUILD_DIR=/tmp/pio_build esp32-factory pio run
