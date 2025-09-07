# IoT Based Drive

## A virtual drive implementation that uses a regular Linux directory as storage while distributing the actual file data across network-connected IoT devices in a home network. Designed to maximize storage utilization and improve security through decentralized file storage.

## Motivation

This project is designed to leverage unused storage space on common household IoT devices — such as smart air conditioner controllers, smart plugs, Raspberry Pi devices, and more.

- Data Accessible only within the Local Area Network (LAN)
- Implement RAID 01, therefore the system ensures thata data is:
  - **Secure**
  - **Backed up**

---

## Introduction

IoT-Based Drive is a user-space application for Linux systems. It allows users to mount a virtual drive using the standard `mount` shell command.

Data is transmitted via TCP from the kernel space to the user space using the Linux **Network Block Device (NBD)** module.

> This project was developed as the final assignment in the Infinity Labs R&D Software Development trainning.

---

## Setup & Usage

### Compilation with Makefile

To build the project:

```bash
cd IoT-Drive
make
```

> Compiled executables will be in the `IoT-Drive/bin/release` directory for release mode and `IoT-Drive/bin/debug` directory for debug mode.

executables files:

1. master.out
2. minion1.out
3. minion2.out
4. minion3.out

- Each **minion** device run his own port(8001-8003)

### Mounting the Drive

1. Load the NBD kernel module:

```bash
sudo modprobe nbd
echo 4 | sudo tee /sys/block/nbd*/queue/max_sectors_kb
```

2. In a **second terminal**, run the master executable:

```bash
sudo ./master.out
```

3. Back in the **first terminal**, format and mount the virtual drive:

```bash
sudo mkfs.ext2 /dev/nbd0
sudo mount /dev/nbd0 ~/iot_drive/mount/
```

4. Run the minions executable, each in a **separate terminal**.

```bash
sudo ./minion.out
```

> Your drive is now mounted. Any file operations inside `~/iot_drive/mount/` will be distributed across the minions.
