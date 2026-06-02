# oBattery

A lightweight, simple C program to check and display the battery percentage of a Linux system.

## Overview

oBattery reads battery information directly from Linux system files (`/sys/class/power_supply/`) and displays the current battery percentage. It's designed to be minimal, fast, and dependency-free.

## Features

- Lightweight and minimal dependencies
- Written in pure C
- Simple and accurate battery percentage calculation
- Linux-only (uses `/sys/class/power_supply/`)
- Fast execution
- Easy to integrate into shell scripts or status bars

## Prerequisites

- GCC compiler
- Linux system with battery support
- Standard C library (libc)

## Installation

### From AUR (Arch User Repository)

If you're using Arch Linux or an Arch-based distribution, you can install oBattery directly from the AUR:

**Using yay:**
```sh
yay -S obattery
```

**Using paru:**
```sh
paru -S obattery
```

**Using makepkg:**
```sh
git clone https://aur.archlinux.org/obattery.git
cd obattery
makepkg -si
```

### Manual Compilation

#### Compile

```sh
gcc -o obattery code.c
```

#### Install (Optional)

```sh
sudo install -Dm755 obattery /usr/local/bin/obattery
```

#### Run

```sh
./obattery
```

Or if installed system-wide:
```sh
obattery
```

## Usage

Simply execute the binary to display your current battery percentage:

```sh
obattery
```

### Output

The program will display the battery percentage as a simple number or message.

## How It Works

oBattery reads from the Linux power supply interface:
- `/sys/class/power_supply/BAT0/energy_current` - Current battery energy
- `/sys/class/power_supply/BAT0/energy_full` - Full battery capacity

It calculates the percentage as: `(current_energy / full_energy) × 100`

## Integration

### With i3statusbar, Polybar, or Dwmblocks

Add to your status bar configuration:

```sh
./path/to/obattery
```

### With Shell Scripts

```bash
BATTERY=$(obattery)
echo "$BATTERY"
```

## Troubleshooting

**Battery not detected:**
- Verify `/sys/class/power_supply/` exists on your system
- Check if `BAT0` is the correct battery device name (may vary by system)
- Run `ls /sys/class/power_supply/` to see available devices

## License

Include your license information here (e.g., MIT, GPL, etc.)

## Contributing

Contributions are welcome! Feel free to submit issues or pull requests.

## Author

spyflow

---

Made for Linux users.
