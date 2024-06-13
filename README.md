# ICRS Printer Release Button

This project involves the development of a release button for an ICRS printer using an ESP32C3 development board and Arduino framework. This README provides an overview of the project structure, its components, and wiring details.

## Project Structure

The project is organized as follows:

### Arduino

- **.vscode/**: Configuration files for Visual Studio Code.
- **include/**: Header files for the project.
- **lib/**: Libraries used by the project.
- **src/**: Source code for the project.
- **test/**: Test files for the project.
- **.gitignore**: Specifies files and directories to be ignored by git.
- **platformio.ini**: PlatformIO configuration file.

### Model

- **ESP32Board/**: Contains files related to the ESP32 board model.
- **Exports/**: Exported files for the project.
- **LED/**: LED-related files and models.
- **Renders/**: Rendered images or files for the project.
- **Switch/**: Files related to the switch mechanism.
- **Base.SLDPRT**: SolidWorks part file for the base of the assembly.
- **ButtonAssembly.SLDASM**: SolidWorks assembly file for the button.
- **Cover.SLDPRT**: SolidWorks part file for the cover.

## Wiring

The wiring for the ESP32C3 development board is as follows:

- Green LED wire to D1
- Red LED wire to D2
- Push switch on D4 and D5
- LED ground to GND

## Getting Started

To get started with this project, follow these steps:

1. **Clone the repository**:
    ```sh
    git clone https://github.com/ICRS/icrs_printer_release_button.git
    ```
2. **Open the project in Visual Studio Code**.
3. **Build and upload the firmware** using PlatformIO.

## Dependencies

Ensure you have the following installed:

- [PlatformIO](https://platformio.org/)
- [Visual Studio Code](https://code.visualstudio.com/)
- [Arduino framework](https://www.arduino.cc/)

## License

This project is licensed under the MIT License. See the LICENSE file for more details.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.
