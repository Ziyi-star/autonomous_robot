# Autonomous Line-Following Robot

## Project Overview

This repository contains all the necessary software for controlling an autonomous line-following robot designed as part of the Embedded Systems Laboratory at Universität Kassel under the supervision of Herr Benjamin Herwig. The robot is programmed to navigate a line track for three rounds and perform various tasks as dictated by the project requirements.

### Features

- Navigates a predefined line track for three consecutive rounds.
- Uses line-following sensors to stay on track.
- Communicates its status and actions via USART.
- Capable of pausing, resuming navigation and Rotation based on commands.
- Implements a watchdog timer for automatic reset after completing its tasks.

## Getting Started

### Prerequisites

- AVR-GCC compiler for building the project.
- AVRDUDE or a similar tool for flashing the firmware to the microcontroller.
- An AVR microcontroller ATmega328/P, set up with necessary peripherals (motors, sensors, etc.).

### Installation

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/Ziyi-star/autonomous_robot.git
   cd autonomous-robot
   ```

2. **Compile the Source Code:**
   Using the provided Makefile, compile the source code:
   ```bash
   make all
   ```

3. **Flash the Firmware:**
   Load the compiled firmware onto the microcontroller:
   ```bash
   make flash
   ```

### Configuration

Configure your hardware setup and microcontroller settings in `config.h` as required to match your specific hardware.

## Usage

After the firmware is successfully flashed:
- Place the robot at a designated start point on the track.
- Power on the robot.
- Connect to the robot via Bluetooth and open a serial connection using Cutecom or a similar program.
- Send commands to control the robot as specified:
  - **S:** Start navigation.
  - **P:** Pause/resume navigation.
  - **T:** Test rotation functionality.
  - **H:** restore from rotation functionality and keeps navigation.

## Documentation

Documentation is generated using Doxygen in HTML format. To generate the documentation:

```bash
make doc
```

Ensure you do not submit the generated documentation folder. It should be generated after cloning the repository using the above command.

## Contributing

Individual contributions are welcome. Please fork the repository and submit pull requests for any enhancements.

## Authors

- **[Ziyi Liu ](https://github.com/Ziyi-star/autonomous_robot.git)** - Initial work and ongoing maintenance

## License

This project is licensed under the MIT License - see the LICENSE.md file for details.

## Acknowledgments

- Prof. Dr. B. Sick for project  requirements.
- Benjamin Herwig and all tutors for their guidance, support and assistance.
