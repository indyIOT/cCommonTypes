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
