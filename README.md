Proyecto Final Ambientes Inteligentes (ESPOL)
---

## Overview
This project involves the development of an automated irrigation system that uses temperature and humidity sensors to monitor environmental conditions and determine the precise irrigation needs of a crop. The system integrates with Telegram to allow for remote and dynamic interaction, sending notifications about irrigation status and soil conditions while allowing for manual control if needed.

## Features
- **Sensor-Based Irrigation**: Automatically adjusts irrigation levels based on real-time data from temperature and humidity sensors.
- **Remote Monitoring and Control**: Uses a Telegram bot for notifications and manual control.
- **Simulation and Testing**: Includes a Cisco Packet Tracer simulation to demonstrate the system's functionality with multiple sensors.
- **Educational and Practical Application**: Designed to provide agriculture students with hands-on experience in implementing automation technologies in a real-world context.

## Repository Contents
- **Arduino Code:**
  - `AgriBot.ino`: Full programming for the automated irrigation system, including sensor data collection and irrigation control logic.
  - `AgriBotVisual.ino`: Code for creating a Telegram bot to send and receive commands for monitoring and controlling the irrigation system remotely.
- **Cisco Packet Tracer Simulation:**
  - `ProyectoAmbientes.pkt`: Simulation file demonstrating the system's functionality using multiple sensors.
- **Final Project Report:**
  - `ReporteFinalAI.pdf`: Comprehensive report detailing the project's design, implementation, testing, and results.

## Getting Started
1. **Clone the Repository:**
   ```bash
   git clone https://github.com/davidromeroy/RiegoInteligente.git


2. **Upload Arduino Code**
- Open the `AgriBot.ino` file in the Arduino IDE and upload it to your microcontroller.
- Open the `AgriBotVisual.ino` file in the Arduino IDE, configure it with your Telegram bot token, and upload it to the microcontroller.

3. **Set Up Telegram Bot**
- Follow the instructions provided in `AgriBot.ino` to create a new bot on Telegram and get your bot token.
- Enter your bot token in the `AgriBot.ino` file and upload the code to the Arduino.

4. **Run Simulation**
- Open the `ProyectoAmbientes.pkt` file in Cisco Packet Tracer to explore the simulation environment and test the system's functionality.

## How It Works
- The **Arduino code** collects data from temperature and humidity sensors and controls the irrigation valves based on predefined thresholds.
- The **Telegram bot** provides a user interface for monitoring the system's status and manually controlling the irrigation valves.
- The **Packet Tracer simulation** allows testing and visualization of the system in a simulated environment, ensuring all components work correctly before deployment.
## Requirements
### Hardware
- Esp32 
- Temperature and humidity sensors
- Relay module for controlling irrigation valves
- Wi-Fi module for communication (included in esp32)
### Software
- Arduino IDE
- Cisco Packet Tracer (for simulation)
- Telegram app (for bot interaction)
## Future Improvements
- Integration of additional sensors for enhanced monitoring (e.g., soil pH, moisture depth).
- Implementation of machine learning algorithms for predictive irrigation.
- Development of a dedicated mobile app for better user experience.
## Contributing
Contributions are welcome! Please feel free to submit a pull request or open an issue for suggestions or improvements.
## License
This project is licensed under the MIT License - see the LICENSE file for details.
