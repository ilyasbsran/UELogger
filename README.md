# UELogger

UELogger is a lightweight, universal, and portable logging library designed for embedded systems. It can be easily used on microcontroller-based architectures such as ARM Cortex-M (STM32, NXP, Nordic, etc.), RISC-V, and ESP32.

Logging can be performed over UART, SWO, RTT, or standard printf outputs. The library supports ANSI-colored log levels, application name injection, timestamping, and log retention during system crashes or hard faults.

## Features

## Features

- **Lightweight and portable:** Designed to run with minimal memory usage on embedded systems.
- **ANSI color support:** Provides colored log levels in the console (can be disabled).
- **Multiple log levels:** Supports DEBUG, INFO, WARN, and ERROR levels.
- **Configurable log filtering:** Messages below the `ueLoggerCurrentLogLevel` are automatically skipped.
- **Bypass format option:** When `UELOGGER_BYPASS_FORMAT` is defined, logs are printed directly without format string processing.
- **Timestamp support:** Adds millisecond-based timestamps via UELoggerGetMilliseconds().
- **File, function, and line tracing:** Includes file name, function name, and line number in logs.
- **Application name tagging:** Adds the application name to log headers using UELoggerSetAppName().
- **Hard Fault Logger:** When `UELOGGER_HARD_FAULT_ENABLED` is active, retains critical logs before system crash.
- **Flexible output management:** Output can be redirected to UART, RTT, SWO, or custom-defined functions using UELoggerRegisterAppFunctions().
- **Minimal dependencies:** Relies only on standard C library functions like snprintf and memset.
- **STM32CubeProgrammer SWV support:** Works with STM32CubeProgrammer’s Serial Wire Viewer (SWV), including optional colorized traces via #GRN#, #RED#, and #ORG# markers when “Activate colors” is enabled.

## Prerequisites
Before using the UELogger library, ensure the following requirements are met:

- **C99-compliant compiler** (e.g., GCC, ARMCC, IAR).
- **Standard C library functions:** The compiler must support basic functions such as snprintf, memset, and strlen.
- **UART or equivalent output interface:** A custom output function (e.g., printf or UART) must be registered using UELoggerRegisterAppFunctions().
- **Timestamp provider:** The UELoggerGetMilliseconds() function must be implemented in the application to provide millisecond-level system time.
- **No RTOS dependency:** Works with FreeRTOS, bare-metal systems, or other operating systems.

## Usage

#### 1. Add UELogger as a Submodule

To include UELogger in your project, add it as a Git submodule:
```bash
git submodule add <repo-url> uelogger
git submodule update --init --recursive
```
This will place UELogger under the uelogger/ directory in your repository.

#### 2. Add Include Paths
Once UELogger is added, you need to include its headers in your project.
**Eclipse-based IDE Example:**

**1.** Right-click your project and go to Properties.
**2.** Navigate to C/C++ General → Paths and Symbols → Includes.
**3.** Add the path to the UELogger headers, e.g.,
```bash
${ProjDirPath}/uelogger
```
**4.** Apply and close.

If you are using a Makefile or CMake, you can add:
```bash
CFLAGS += -I./uelogger
```
or
```bash
include_directories(${CMAKE_SOURCE_DIR}/uelogger)
```

#### 3. Add Preprocessor Defines
Enable the logger by adding one of the following macros to your build configuration:

- UELOGGER_ENABLED – Enables normal logging.
- UELOGGER_HARD_FAULT_ENABLED – Enables hard fault logging mode (minimal log buffer to catch critical events before crash).
- UELOGGER_COLOR_STM32CUBE_PROGRAMMER – Enables STM32CubeProgrammer SWV color output using #GRN#, #RED#, and #ORG# markers.
- UELOGGER_COLOR_ANSI – Enables ANSI escape sequence color output for supported terminals.
- UELOGGER_BYPASS_FORMAT – Skips format string processing and prints log messages directly.

**Eclipse-based IDE Example:**

**1.** Right-click your project → Properties.
**2.** Go to C/C++ Build → Settings → Tool Settings.
**3.** Under Preprocessor, add:
```bash
UELOGGER_ENABLED
```
or
```bash
UELOGGER_HARD_FAULT_ENABLED
```
**4.** Under Preprocessor, add:
```bash
UELOGGER_COLOR_STM32CUBE_PROGRAMMER
```
or
```bash
UELOGGER_COLOR_ANSI
```
**5.** Apply and rebuild your project.

#### 4. Implement a Wrapper (Recommended)

Although UELogger can be used directly, we strongly recommend creating a wrapper to keep your project-level logging calls consistent and easily configurable.

Example of a simple wrapper (app_logger.c):

```bash
#include "uelogger.h"
#include "stm32g4xx_hal.h"

static UART_HandleTypeDef *uartHandle = NULL;

static void LoggerOutput(const uint8_t *p, size_t len) {
    if (uartHandle) {
        HAL_UART_Transmit(uartHandle, (uint8_t*)p, len, 100);
    }
}

static float LoggerTime(void) {
    return (float)HAL_GetTick();
}

void AppLogger_Init(UART_HandleTypeDef *huart) {
    uartHandle = huart;
    UELoggerSetAppName("MyApp");
    UELoggerSetCurrentLogLevel(DBG);
    UELoggerRegisterAppFunctions(LoggerTime, LoggerOutput);
}
```
If you want to use a custom log macro name (e.g., ZB_printf or LOGGER), you can wrap UELogger’s UELOG macro like this:
```bash
/**
 * @brief This macro redirects formatted log messages to UELogger.
 */
#define ZB_printf(level, ...)  UELOG(level, __VA_ARGS__)
```
**Usage Example:**
```bash
ZB_printf(ERR, "Ilyas Basaran");
UELOGGER(DBG, "Ilyas Basaran");
```

#### 5. Verify the Integration
After building:

- Run the target, and check UART logs (or your chosen output).
- If UELOGGER_ENABLED is active, you’ll see color-coded logs with timestamps.
- If UELOGGER_COLOR_STM32CUBE_PROGRAMMER or UELOGGER_COLOR_ANSI is defined, log output will include colors using the selected format (STM32CubeProgrammer SWV markers or ANSI escape codes).
- If UELOGGER_HARD_FAULT_ENABLED is active, you can call GetLoggerBuffer() to dump logs after a hard fault.
- If `UELoggerCurrentLogLevel` is set, messages below that level will be filtered out and not shown in the logs.
- If UELOGGER_BYPASS_FORMAT is defined, log messages will be printed directly without format string parsing.

#### 6. View the Logs
Depending on your output method, follow one of the flows below.

##### UART (Serial) Output
**1.** Open a serial viewer (e.g., PuTTY, Tera Term, CoolTerm).
**2.** Select the correct COM/USB port for your board.
**3.** Configure the serial settings to match your firmware (e.g., 115200 baud, 8 data bits, no parity, 1 stop bit).
**4.** Open the connection and confirm the terminal is receiving data.
**5.** Observe logs as they arrive.
**6.** If UELOGGOR_COLOR_ANSI is defined and your terminal supports ANSI colors, log levels will be colorized.

##### ITM / SWV (STM32CubeProgrammer)
**1.** Launch STM32CubeProgrammer and open the SWV (Serial Wire Viewer) page.
**2.** Enter the correct target System Clock (HCLK) frequency used by your firmware.
**3.** Select the Stimulus Port used by your code (commonly port 0).
**4.** (Optional) Enable “Activate colors” to colorize traces.
- UELogger uses #GRN#, #RED#, #ORG# markers when UELOGGOR_COLOR_STM32CUBE_PROGRAMMER is defined.
**5.** Start SWV reception and observe logs in the SWV window.

#### 7. Updating the Submodule

UELogger is included as a Git submodule. To update it to the latest version, run the following commands in your project root:

```bash
# Fetch the latest changes from the UELogger repository
git submodule update --remote --merge

# If you want to update and initialize all submodules at once
git submodule update --init --recursive --remote
```
After updating, commit the submodule reference change to your repository:

```bash
git add path/to/UELogger
git commit -m "Update UELogger submodule"
```

## Authors

**İlyas Başaran** - *Creator and Maintainer*

## Contributing and Issue Tracker

Contributions are welcome! Feel free to submit issues or pull requests. If you encounter any problems or have suggestions for improvement, please create an issue in the tracker.

## License

This project is licensed under the Company Internal Use License - see the [LICENSE.md](LICENSE.md) file for details