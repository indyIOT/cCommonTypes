/** ***********************************************
 * @file commonTypes.h
 * @brief Common project-wide types and definitions for the project.
 * @author Anthony Garza
 * @copyright All rights reserved 2026
*************************************************/
#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAX_ERROR_MESSAGE_LENGTH_BYTES
#define MAX_ERROR_MESSAGE_LENGTH_BYTES 100
#endif
#ifndef END_OF_COMMON_ERRORS
#define END_OF_COMMON_ERRORS                                              0x0020
#endif

#ifndef COMMON_ERROR_CODES
#define COMMON_ERROR_CODES
#ifdef ERROR_NONE
#undef ERROR_NONE
#endif
/** Enumeration of Common Error Codes These are always going to be the first error 
 * codes of every Errorcode Enumeration */
typedef enum 
{
    ERROR_NONE                = 0x0000, /* No error */
    ERROR_INVALID_PARAMETER   = 0x0001, /* Invalid parameter */
    ERROR_OUT_OF_MEMORY       = 0x0002, /* Out of memory */
    ERROR_BUFFER_OVERFLOW     = 0x0003, /* Buffer overflow */
    ERROR_BUFFER_UNDERFLOW    = 0x0004, /* Buffer underflow */
    ERROR_NULL_POINTER        = 0x0005, /* Null pointer */
    ERROR_INVALID_STATE       = 0x0006, /* Invalid state */
    ERROR_TIMEOUT             = 0x0007, /* Timeout */
    ERROR_NOT_IMPLEMENTED     = 0x0008, /* Not implemented */
    ERROR_UNKNOWN             = 0x0009, /* Unknown error */
    ERROR_UNINITIALIZED       = 0x000A, /* Uninitialized */
    ERROR_ALREADY_INITIALIZED = 0x000B, /* Object previously been initialized.*/ 
    LAST_COMMON_ERROR_CODE,
} eCommonErrorCodes_t;
#endif

#ifndef SCOMMON_ERROR_COMPACT
#define SCOMMON_ERROR_COMPACT

/**
 * @brief Compact structure for storing error information.
 *  Needs to be 32 bit aligned and packed to 1 byte to ensure that it is compact and can be stored in a circular buffer.
 */
#pragma pack( push, 1 )
typedef struct 
{
    union {
        struct         
        {
            /* data */
            uint16_t _errorCode; /* The error code for this error */
            uint16_t _fileModuleEnum; /* The file module enum where the error occurred */
        };
        uint32_t _errorDetails; /* Combined error code and file module enum for compact storage */
    };
    uint16_t _lineNumber; /* The line number where the error occurred */
    uint8_t  _flags; /* Whether or not this error info is valid */
    uint8_t  _reserved[7]; /* Reserved for future use */
    uint16_t _Unused16; /* Currently unused, reserved for future use */
    uint16_t _crc16; /* CRC16 of the error info for integrity checking */
} sErrorCompact_t;

#ifndef BLANK_ERROR_STRUCT
#define BLANK_ERROR_STRUCT { \
    ._errorCode = ERROR_NONE, \
    ._fileModuleEnum = 0U, \
    ._lineNumber = 0, \
    ._flags = 0, \
    ._reserved = { 0 }, \
    ._Unused16 = 0, \
    ._crc16 = 0 \
} 
#endif
#pragma pack( pop )
#endif
#ifndef SERROR_CODE_MESSAGE_PAIR
#define SERROR_CODE_MESSAGE_PAIR
/** 
 * @brief Structure that contains an error code and its corresponding message.
 */
typedef struct
{
    uint16_t _errorCode; /* The error code */
    char _errorMessage[ MAX_ERROR_MESSAGE_LENGTH_BYTES ]; /* The corresponding error message */  
} sErrorCodeMessagePair_t;
#endif // SERROR_CODE_MESSAGE_PAIR

#ifndef SCOMMON_LOG_CALLBACK
#define SCOMMON_LOG_CALLBACK
typedef enum
{
    LOGGING_TYPE_CRITICAL= 0U,
    LOGGING_TYPE_ERROR   = 1U,
    LOGGING_TYPE_WARNING = 2U,
    LOGGING_TYPE_INFO    = 3U,
    LOGGING_TYPE_DEBUG   = 4U
} eLoggingType_t;


/**
 * @brief Function pointer type for a log callback function.
 * @param logLevel The log level of the message.
 * @param message The log message.
 */ 
typedef sErrorCompact_t (*logCallback_t)(uint16_t moduleId,
                                         uint16_t line,
                                         eLoggingType_t type,
                                         const char *message, ...);
#endif // SCOMMON_LOG_CALLBACK
#ifndef SCOMMON_CREATE_ERROR_CALLBACK
#define SCOMMON_CREATE_ERROR_CALLBACK
typedef sErrorCompact_t (*createErrorCallback_t)( uint16_t errorCode, 
                                                  uint16_t fileModuleEnum, 
                                                  uint16_t lineNumber,
                                                  bool autoStoreError,
                                                  uint8_t const * const errorMessage, 
                                                  uint8_t const * const moduleName );
#endif

#ifndef SCOMMON_DRIVER_CONTROL
#define SCOMMON_DRIVER_CONTROL
/**
 * @brief Function to get the ID of a registered module.
 * @return The module ID of the registered module.
 */
typedef uint16_t (*getModuleIdFunctionPtr_t)( void );
                                              
/**
 * @brief Function to get the version string of the module.
 * @return Pointer to a string containing the version of the module.
 */
typedef uint8_t const * (*getModuleVersionStringFunctionPtr_t)( void );

/**
 * @brief funcrtion to get a module name.
 */
typedef uint8_t const * (*getModuleNameFunctionPtr_t)( void );

/**
 * @brief Function to check if the driver is initialized.
 * @return true if the driver is initialized, false otherwise.
 */
typedef bool (*isDriverInitializedFunctionPtr_t)( void );

typedef enum
{
    STATIC_LIBRARY_BUILD = 0U,
    DYNAMIC_LIBRARY_BUILD = 1U
} eBuildType;

/**
 * @brief Structure common throughout modules for recording version.
 */
typedef struct
{
    uint8_t _major;
    uint8_t _minor;
    uint8_t _patch;
    eBuildType _buildType;
    uint64_t _buildDateTime;    
} sCommonVersionStruct_t;

/**
 * @brief Function to get the current version of the module.
 * @return A structure containing the major, minor, patch, and build type of the module.
 */
typedef sCommonVersionStruct_t (*getModuleVersionFunctionPtr_t)( void );

/**
 * @brief Info structure for a common driver. 
 * This structure contains information about the driver and function pointers 
 * to access that information.
 */
typedef struct
{
    uint8_t const * const _moduleName; /* Name of the module */
    uint8_t const * const _moduleVersionString; /* Version string of the module */
    uint16_t _moduleID; /* ID of the module */
    sCommonVersionStruct_t const _moduleVersion; /* Version of the module */
    bool _isInitialized; /* Whether or not the driver has been initialized */
} sCommonDriverInfoStruct_t;

/**
 * @brief Accessor structure for a common driver. This structure contains
 *  function pointers to access information about the driver.
 */
typedef struct 
{
/* Function pointer to get the module ID */
    getModuleIdFunctionPtr_t getModuleIdFunction; 
 /* Function pointer to get the module version string */
    getModuleVersionStringFunctionPtr_t getModuleVersionStringFunction;
 /* Function pointer to get the module name */
    getModuleNameFunctionPtr_t getModuleNameFunction;
/* Function pointer to get the module version structure */
    getModuleVersionFunctionPtr_t getModuleVersionFunction;
/* Function pointer to check if the driver is initialized */
    isDriverInitializedFunctionPtr_t isDriverInitializedFunction; 
} sCommonDriverAccessorStruct_t;

/**
 * @brief Control structure for a common driver. This structure contains information about the driver and function pointers to access that information.
 */
typedef struct
{
    sCommonDriverInfoStruct_t _driverInfo; /* Information about the driver */
    /* Function pointers to access driver information */
    const sCommonDriverAccessorStruct_t _driverAccessors; 
} sCommonDriverControlStruct_t;


#endif // SCOMMON_DRIVER_CONTROL

#ifndef SCOMMON_MEMORY_FUNCTIONS
#define SCOMMON_MEMORY_FUNCTIONS

/** 
 * @brief Generic Read Memory function, Given an address and a size, 
 *        this function will read the memory at that address and return the value.
 * @param address The address to read from.
 * @param length The number of bytes to read.
 * @param buffer Pointer to a buffer to store the read data.
 * @returns ERROR_NONE if successful, or a uint16_t if it failed.
 */
typedef uint16_t (*readMemoryFunctionPtr_t)( uint32_t const address,
                                             uint8_t * const buffer,
                                             size_t const length );
/**
 * @brief Generic Write Memory function, Given an address and a size,
 * this function will write information to that address and return
 * an error code if it failed.
 * @param address The start address to write too.
 * @param length The number of bytes to write.
 * @param buffer Pointer to a buffer containing the data to write.
 * @param Whether or not a readback should be done to verify.
 * @returns ERROR_NONE if successful, or a uint16_t if it failed.
 */
typedef uint16_t (*writeMemoryFunctionPtr_t)( uint32_t const address, 
                                              uint8_t * const buffer,
                                              size_t const lengthSizeBytes,
                                              bool const verifyWrite );
#endif // SCOMMON_MEMORY_FUNCTIONS
#ifdef __cplusplus
}
#endif



#endif /* COMMON_TYPES_H */
