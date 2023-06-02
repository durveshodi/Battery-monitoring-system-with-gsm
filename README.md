# Battery Monitoring System with GSM

This project implements a Battery Monitoring System using an Arduino board and a GSM module. It allows you to monitor the battery voltage remotely by sending SMS alerts. The system can be useful in various applications where battery voltage monitoring is critical, such as solar power systems, backup power supplies, and remote monitoring setups.


## Features

- Real-time monitoring of battery voltage
- SMS alerts when battery voltage is below a threshold
- Configurable voltage threshold for triggering alerts
- Battery voltage displayed on an LCD screen
- Easy integration with Arduino boards and GSM modules

## Components

To build this project, you will need the following components:

- Arduino board (e.g., Arduino Uno)
- GSM module (e.g., SIM800L)
- LCD screen (16x2 or 20x4)
- Resistors and capacitors as per circuit requirements
- Jumper wires
- Breadboard or PCB for circuit assembly
- Power source (battery or power supply)
- SIM card for GSM module

## Configuration

The Battery Monitoring System can be configured by modifying the variables in the sketch:

- `thresholdVoltage`: Set the voltage threshold for triggering SMS alerts. Adjust this value according to your battery specifications.
- `phoneNumber`: Enter the phone number to which SMS alerts should be sent. Make sure to include the country code.

Make sure to save the modified sketch and upload it to the Arduino board after making any changes.

## Usage

Once the system is set up and configured, it will continuously monitor the battery voltage. The LCD screen will display the current voltage, and SMS alerts will be sent if the voltage drops below the specified threshold.

You can monitor the battery voltage remotely by checking the SMS alerts received on the configured phone number. Additionally, you can connect the Arduino board to a computer and open the Serial Monitor in the Arduino IDE to view the voltage readings.

## Contributing

Contributions to this project are welcome! If you have any ideas, suggestions, or improvements, feel free to open an issue or submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE). You are free to modify, distribute, and use it for personal or commercial purposes.

## Acknowledgements

- [Arduino](https://www.arduino.cc/) - Open-source electronics platform
- [SIM800L Library](https://github.com/avishorp/TinyGSM) - Library for interfacing with the SIM800L GSM module
- [LiquidCrystal Library](https://www.arduino.cc/en/Reference/LiquidCrystal) - Arduino library for controlling
