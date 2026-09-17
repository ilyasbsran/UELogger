/*
 * uelogger.c
 *
 *  Created on: Dec 26, 2023
 *      Author: İlyas Başaran
 */

#include <uelogger.h>

#if defined(UELOGGER_ENABLED) || defined(UELOGGER_HARD_FAULT_ENABLED)

#if defined(UELOGGER_COLOR_STM32CUBE_PROGRAMMER) && defined(UELOGGER_COLOR_ANSI)
  #error "Only one color mode can be selected: UELOGGER_COLOR_STM32CUBE_PROGRAMMER or UELOGGER_COLOR_ANSI"
#endif

/**
 * @brief This variable represents the maximum length that can be logged.
 */
#define UELOGGER_BUFFER_MAX_LENGTH                                						128

/**
 * @brief Size of the application name buffer.
 *
 * @details This constant defines the size of the character array used to store
 *          the application name. Adjust this value based on the maximum length
 *          your application supports for the application name.
 */
#define UELOGGER_APP_NAME_SIZE 														    10

/**
 * @brief Contains the string form of the logs.
 *
 * The constant used by @ref UELOG when the user wants to print log.
 */
static __attribute__((unused)) char ueLoggerBuffer[UELOGGER_BUFFER_MAX_LENGTH];

/**
 * @brief Current log level for the application.
 *
 * @details This global variable represents the current log level for the application.
 *          It is initialized to WARN, indicating that log messages with a level
 *          of WARN or higher will be printed by default.
 */
uelogger_levels_t ueLoggerCurrentLogLevel = WARN;

/**
 * @brief Buffer to store the application name.
 *
 * @details This character array is used to store the application name. The size of
 *          the array is determined by the UELOGGER_APP_NAME_SIZE constant. The array is
 *          initialized with the default value "UELOGGER". Adjust the size and initial
 *          value based on your application requirements.
 */
static __attribute__((unused)) char ueLoggerAppName[UELOGGER_APP_NAME_SIZE] = "UELOGGER";

/**
 * @brief Function pointer to obtain the elapsed time in milliseconds.
 *
 * @details This function pointer is used to hold the address of a function that returns
 *          the elapsed time in milliseconds as a floating-point value (float).
 *          The function is typically set by calling UELoggerRegisterAppFunctions.
 */
static UELoggerGetMillisecondsFunction pfUELoggerGetMilliseconds = NULL;

/**
 * @brief Function pointer to print formatted log messages.
 *
 * @details This function pointer is used to hold the address of a function that prints
 *          formatted log messages. It takes a pointer to a buffer (uint8_t*) and the
 *          length of the buffer (uint8_t) as parameters. The function is typically set
 *          by calling UELoggerRegisterAppFunctions.
 */
static UELoggerPrintfFunction pfUELoggerPrintf = NULL;
#endif

/**
 * @brief   Get the elapsed time in milliseconds.
 *
 * @details This function retrieves the elapsed time in milliseconds through the function
 *          pointer pfUELoggerGetMilliseconds. If pfUELoggerGetMilliseconds is set (non-NULL),
 *          it calls the corresponding function to obtain the elapsed time; otherwise, it returns
 *          a default error value of 0f.
 *
 * @return  The elapsed time in milliseconds, or 0f if the function pointer is not set
 *          or the corresponding function fails to provide a valid value.
 */
__inline float UELoggerGetMilliseconds(void) {
#if defined(UELOGGER_ENABLED) || defined(UELOGGER_HARD_FAULT_ENABLED)

    if (pfUELoggerGetMilliseconds != NULL) {
        return pfUELoggerGetMilliseconds();
    }

#endif

    return 0.0f;
}

/**
 * @brief   Print a formatted log message.
 *
 * @details This function prints a formatted log message using the function pointer
 *          pfLoggerPrintf. If pfLoggerPrintf is set (non-NULL), it calls the corresponding
 *          function to print the log message; otherwise, it does nothing.
 *
 * @param[in] p   A pointer to the buffer containing the log message.
 * @param[in] len The length of the log message buffer.
 */
__inline void UELoggerPrintf(const uint8_t* p, size_t len) {
#if defined(UELOGGER_ENABLED) || defined(UELOGGER_HARD_FAULT_ENABLED)

    if (pfUELoggerPrintf != NULL) {
        pfUELoggerPrintf(p, len);
    }

#endif
}

/**
 * @brief   Register application-specific functions for logging and information retrieval.
 *
 * @details This function is used to register application-specific functions for information
 *          retrieval and logging. It takes function pointers to the GetMilliseconds and
 *          LoggerPrintf functions as parameters and assigns them to
 *          corresponding internal function pointers.
 *
 * @param[in] pUELoggerGetMilliseconds      A function pointer to obtain the elapsed time in milliseconds.
 * @param[in] pUELoggerPrintf               A function pointer to print a formatted log message.
 */
void UELoggerRegisterAppFunctions(UELoggerGetMillisecondsFunction pUELoggerGetMilliseconds, UELoggerPrintfFunction pUELoggerPrintf) {
#if defined(UELOGGER_ENABLED) || defined(UELOGGER_HARD_FAULT_ENABLED)

    pfUELoggerGetMilliseconds = pUELoggerGetMilliseconds;
    pfUELoggerPrintf = pUELoggerPrintf;

#endif
}

/**
 * @brief   Set the application name.
 *
 * @details This function sets the application name using the provided string `appName`.
 *          It only sets the name if its length does not exceed `UELOGGER_APP_NAME_SIZE - 1`.
 *          If the string is longer, the function does nothing.
 *
 * @param[in] appName   A pointer to the string representing the new application name.
 */
void UELoggerSetAppName(const char* appName) {
#if defined(UELOGGER_ENABLED) || defined(UELOGGER_HARD_FAULT_ENABLED)

    // Check if the given string fits in the buffer
    if (strlen(appName) >= UELOGGER_APP_NAME_SIZE) {
        // Do not set if it exceeds the buffer size
        return;
    }

    // Copy the string safely
    strncpy(ueLoggerAppName, appName, UELOGGER_APP_NAME_SIZE - 1);
    ueLoggerAppName[UELOGGER_APP_NAME_SIZE - 1] = '\0';

#endif
}

/**
 * @brief   Set the current log level.
 *
 * @details This function sets the current log level to the specified value `level`
 *          in the global variable `ueLoggerCurrentLogLevel`.
 *
 * @param[in] level   The new log level to be set, typically of the type uelogger_levels_t.
 */
void UELoggerSetCurrentLogLevel(uelogger_levels_t level) {
#if defined(UELOGGER_ENABLED) || defined(UELOGGER_HARD_FAULT_ENABLED)

    // Set the current log level to the specified value.
    ueLoggerCurrentLogLevel = level;

#endif
}

/**
 * @brief   Retrieve the current content of the logger buffer.
 *
 * @details This function copies the current content of the logger buffer into
 *          the provided buffer. The logger buffer stores formatted log messages
 *          and related information.
 *
 * @param[out] buffer   A pointer to the buffer where the logger content will be copied.
 */
void UELoggerGetLoggerBuffer(char *buffer) {
#if defined(UELOGGER_HARD_FAULT_ENABLED)
    // Copy the content of the logger buffer into the provided buffer.
    memcpy(buffer, &ueLoggerBuffer, sizeof(ueLoggerBuffer));
#endif
}

#if defined(UELOGGER_ENABLED)

/**
 * @brief Macro for adding an entity to the description array.
 *
 * Macro that helps to create a single entity in the description array.
 */
#define UELOGGER_ENTITY(mnemonic)                                						{.level=mnemonic, .name=#mnemonic}

/**
 *  Macro element that describes an log level. @ref LOGGER_ENTITY
 */
typedef struct{
    uelogger_levels_t level;                                                      		/**< Log level */
    char const * name;                                                                  /**< Log level name */
}uelogger_entity_t;

/**
 * @brief Array entity element that describes an log.
 */
typedef struct
{
    uelogger_entity_t entity;                                                     		/**< @ref uelogger_entity_t */
    char const * color;                                                                 /**< Log level color code */
}uelogger_desc_t;

#if defined(UELOGGER_COLOR_STM32CUBE_PROGRAMMER)

/**
 * @brief Array with log level codes.
 *
 * Array that describes log levels.
 *
 * @note Log levels do not need to be in ascending order in this string.
 *       This is checked in automated unit testing prior to release.
 */
static __attribute__((unused)) uelogger_desc_t const ueLoggerLevelCodes[] = {
    {
        .entity = UELOGGER_ENTITY(DBG),
        .color = "#GRN" ///< Green color for DEBUG log level.
    },
    {
        .entity = UELOGGER_ENTITY(INFO),
        .color = "#GRN" ///< Green color for INFO log level.
    },
    {
        .entity = UELOGGER_ENTITY(WARN),
        .color = "#ORG#" ///< Orange color for WARN log level.
    },
    {
        .entity = UELOGGER_ENTITY(ERR),
        .color = "#RED#" ///< Red color for ERR log level.
    },
    {
        .entity = UELOGGER_ENTITY(FATAL),
        .color = "#RED#" ///< Red color for FATAL log level.
    }
};

#elif defined(UELOGGER_COLOR_ANSI)

/**
 * @brief Array with log level codes.
 *
 * Array that describes log levels.
 *
 * @note Log levels do not need to be in ascending order in this string.
 *       This is checked in automated unit testing prior to release.
 */
static __attribute__((unused)) uelogger_desc_t const ueLoggerLevelCodes[] = {
    {
        .entity = UELOGGER_ENTITY(DBG),
        .color = "\x1b[37;1m" ///< White color for DEBUG log level.
    },
    {
        .entity = UELOGGER_ENTITY(INFO),
        .color = "\x1b[32;1m" ///< Green color for INFO log level.
    },
    {
        .entity = UELOGGER_ENTITY(WARN),
        .color = "\x1b[33;1m" ///< Yellow color for WARN log level.
    },
    {
        .entity = UELOGGER_ENTITY(ERR),
        .color = "\x1b[31;1m" ///< Red color for ERR log level.
    },
    {
        .entity = UELOGGER_ENTITY(FATAL),
        .color = "\x1b[31;1m" ///< Red color for FATAL log level.
    }
};

#else

/**
 * @brief Array with log level codes.
 *
 * Array that describes log levels.
 *
 * @note Log levels do not need to be in ascending order in this string.
 *       This is checked in automated unit testing prior to release.
 */
static __attribute__((unused)) uelogger_desc_t const ueLoggerLevelCodes[] = {
    {
        .entity = UELOGGER_ENTITY(DBG),
        .color = ""
    },
    {
        .entity = UELOGGER_ENTITY(INFO),
        .color = ""
    },
    {
        .entity = UELOGGER_ENTITY(WARN),
        .color = ""
    },
    {
        .entity = UELOGGER_ENTITY(ERR),
        .color = ""
    },
    {
        .entity = UELOGGER_ENTITY(FATAL),
        .color = ""
    }
};

#endif

#if defined(UELOGGER_COLOR_ANSI)

/**
 * @brief A literal string containing the color reset and newline characters.
 */
static __attribute__((unused)) const char* RESET_NEWLINE = "\x1b[0m\r\n";

#else

/**
 * @brief A literal string containing the  newline characters.
 */
static __attribute__((unused)) const char* RESET_NEWLINE = "\r\n";

#endif

/**
 * @brief   Extracts the file name from a given file path.
 *
 * @details This function takes a file path as input and identifies the last occurrence
 *          of '/' or '\\' to determine the file name. It returns a pointer to the substring
 *          containing the file name.
 *
 * @param[in] path A pointer to the file path string.
 *
 * @return  A pointer to the substring containing the file name, or the original path if
 *          no separator ('/' or '\\') is found.
 */
static __attribute__((unused)) const char* GetFileNameFromPath(const char* path) {
    // Find the last occurrence of '/' or '\\' in the path
    const char* lastSlash = strrchr(path, '/');
    const char* lastBackslash = strrchr(path, '\\');

    // Choose the last occurrence as the separator
    const char* lastSeparator = (lastSlash > lastBackslash) ? lastSlash : lastBackslash;

    // If a separator is found, return the substring after it; otherwise, return the whole path
    return (lastSeparator != NULL) ? lastSeparator + 1 : path;
}

/**
 * @brief This function prints the string to be printed and the argument with the information of the place where it
 *        was printed.
 *
 * @param level         Log level
 * @param file          The name of the file in which the file is used.
 * @param function      The name of the file in which the function is used.
 * @param line          The name of the file in which the line is used.
 * @param fmt           The string to be printed.
 * @param ...           The argument to be printed.
 *
 */
void UELOG(int level, const char *file, const char *function, int line, const char *fmt, ...) {
    // Skip logging if the message level is lower than the current log level
    if (level < ueLoggerCurrentLogLevel)
        return;

    uint16_t bufSize = sizeof(ueLoggerBuffer);
    uint16_t used = 0;

    // A macro to safely append formatted text to the ueLoggerBuffer
    // If the formatted text does not fit, the buffer is flushed using LoggerPrintf
    // and then cleared with memset before retrying.
    #define ADD_FMT(...) do { \
		/* Write formatted string to the buffer starting at position 'used' */ \
		int len = snprintf(ueLoggerBuffer + used, bufSize - used, __VA_ARGS__); \
		\
		/* If snprintf returns a negative value, an error occurred -> exit function */ \
		if (len < 0) return; \
		\
		/* Check if the new content would exceed the buffer size */ \
		if (used + len >= bufSize) { \
			/* Flush the current buffer contents (send the log out) */ \
			UELoggerPrintf((uint8_t*)ueLoggerBuffer, used); \
			\
			/* Clear the buffer to start fresh */ \
			memset(ueLoggerBuffer, 0, bufSize); \
			used = 0; \
			\
			/* Retry writing the formatted string to the fresh buffer */ \
			len = snprintf(ueLoggerBuffer + used, bufSize - used, __VA_ARGS__); \
			\
			/* If it still fails, exit function */ \
			if (len < 0) return; \
		} \
		\
		/* Advance the 'used' counter by the number of characters added */ \
		used += len; \
	} while(0)

#if !defined(UELOGGER_BYPASS_FORMAT)
#if defined(UELOGGER_COLOR_STM32CUBE_PROGRAMMER) || defined(UELOGGER_COLOR_ANSI)

    // Add the log color prefix (e.g., for terminal output highlighting)
    ADD_FMT("%s", ueLoggerLevelCodes[level].color);

#endif

    // Add the application name to the log prefix
    ADD_FMT("%s", ueLoggerAppName);

    // Add the current time in milliseconds (formatted with 3 decimal places)
    ADD_FMT("[%.3f] : ", UELoggerGetMilliseconds());

    // Add the log level or entity name (e.g., INFO, ERROR, etc.)
    ADD_FMT("%s : ", ueLoggerLevelCodes[level].entity.name);

    // Add the file name (extracted from the full file path)
    ADD_FMT("%s : ", GetFileNameFromPath(file));

    // Add the function name where the log was called
    ADD_FMT("%s : ", function);

    // Add the line number in the source code
    ADD_FMT("%d -> ", line);
#endif
    // Add the formatted log message (main content)
    va_list args;
    va_start(args, fmt);
    // Format the variable arguments into the buffer starting from 'used'
    int len = vsnprintf(ueLoggerBuffer + used, bufSize - used, fmt, args);
    va_end(args);

    if (len > 0) {
        // Check if the new content (current + new message) would exceed the buffer size
        if (used + len >= bufSize) {
            // Flush the current buffer contents (send the log message so far)
            UELoggerPrintf((uint8_t*)ueLoggerBuffer, used);

            // Clear the buffer and reset 'used' to start writing from the beginning
            memset(ueLoggerBuffer, 0, bufSize);
            used = 0;

            // Reformat the same message again into the empty buffer
            va_start(args, fmt);
            vsnprintf(ueLoggerBuffer + used, bufSize - used, fmt, args);
            va_end(args);
        }

        // Update 'used' to reflect the number of characters just added
        used += len;
    }
#if !defined(UELOGGER_BYPASS_FORMAT)
    // Add the RESET_NEWLINE sequence (resets terminal color and moves to a new line)
    ADD_FMT("%s", RESET_NEWLINE);
#else
    ADD_FMT("%s", "\r\n");
#endif
    // Flush any remaining data in the buffer
    if (used > 0) {
        // Send the current buffer content (log message)
        UELoggerPrintf((uint8_t*)ueLoggerBuffer, used);

        // Clear the buffer for the next log message
        memset(ueLoggerBuffer, 0, bufSize);
    }

    // Undefine the macro to avoid accidental reuse outside of this function
    #undef ADD_FMT
}

#elif defined(UELOGGER_HARD_FAULT_ENABLED)

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief   Extracts the file name from a given file path.
 *
 * @details This function takes a file path as input and identifies the last occurrence
 *          of '/' or '\\' to determine the file name. It returns a pointer to the substring
 *          containing the file name.
 *
 * @param[in] path A pointer to the file path string.
 *
 * @return  A pointer to the substring containing the file name, or the original path if
 *          no separator ('/' or '\\') is found.
 */
static __attribute__((unused)) const char* GetFileNameFromPath(const char* path) {
    // Find the last occurrence of '/' or '\\' in the path
    const char* lastSlash = strrchr(path, '/');
    const char* lastBackslash = strrchr(path, '\\');

    // Choose the last occurrence as the separator
    const char* lastSeparator = (lastSlash > lastBackslash) ? lastSlash : lastBackslash;

    // If a separator is found, return the substring after it; otherwise, return the whole path
    return (lastSeparator != NULL) ? lastSeparator + 1 : path;
}

/**
 * @brief   Log a message with file and line information.
 *
 * @details This function logs a message with information about the source file
 *          and line number where the log occurred. It appends the file name,
 *          line number, and a separator to the logger buffer. The stored logs
 *          in the buffer are managed to prevent overflow.
 *
 * @param[in] file   A pointer to the source file name.
 * @param[in] line   The line number in the source file.
 */
void UELOG(const char *file, int line) {
    // Define the character used to separate entries in the buffer (e.g., "main.c100-")
    const char separator = '-';

    // ---------- Step 1: Calculate the length of the line number as a string ----------
    // First, convert the line number into its absolute value to count digits
    int temp_line = (line < 0) ? -line : line;

    // Initialize line length to 1 if the number is 0 or negative
    int line_len = (line <= 0) ? 1 : 0;

    // Count how many digits the line number has
    while (temp_line != 0) {
        temp_line /= 10;
        line_len++;
    }

    // Add one more character if the line number is negative (for the minus sign)
    if (line < 0) line_len++;

    // ---------- Step 2: Get the file name and calculate its length ----------
    // Remove any directory path to keep only the filename
    const char *filename = GetFileNameFromPath(file);

    // Calculate how many characters the filename contains
    int filename_len = strlen(filename);

    // Calculate how many total characters the new log entry will need:
    // filename + line number (as string) + separator (e.g., main.c100-)
    int new_entry_len = filename_len + line_len + 1; // +1 for separator

    // ---------- Step 3: Check if there is space in the buffer ----------
    // If not enough space, remove oldest entries (FIFO behavior)
    while (new_entry_len > UELOGGER_BUFFER_MAX_LENGTH - strlen(ueLoggerBuffer)) {
        // Find the first separator character (i.e., end of the first entry)
        char *first_sep = strchr(ueLoggerBuffer, separator);

        // If no separator is found, reset the buffer — possible corruption
        if (first_sep == NULL) {
            memset(ueLoggerBuffer, 0x00, UELOGGER_BUFFER_MAX_LENGTH);
            break;
        }

        // Calculate how many bytes to remove to shift the buffer left
        int remove_len = (first_sep - ueLoggerBuffer) + 1;

        // Move all data after the removed entry to the beginning of the buffer
        memmove(ueLoggerBuffer, ueLoggerBuffer + remove_len, UELOGGER_BUFFER_MAX_LENGTH - remove_len);

        // Zero out the trailing space at the end of the buffer
        memset(ueLoggerBuffer + UELOGGER_BUFFER_MAX_LENGTH - remove_len, 0x00, remove_len);
    }

    // ---------- Step 4: Append the new log entry ----------
    // Get the current length of the buffer (to append at the end)
    int current_len = strlen(ueLoggerBuffer);

    // Write the new log entry into the buffer in the format: filename + line + separator
    snprintf(ueLoggerBuffer + current_len, UELOGGER_BUFFER_MAX_LENGTH - current_len,
             "%s%d%c", filename, line, separator);
}

#else

#endif
