/*
 * QC3Control.h
 *
 * Modified: 5/28/2021 10:12:27 PM
 *  Author: IR
 *  Library: https://github.com/vdeconinck/QC3Control
 */ 

#ifndef QC3CONTROL_H_
#define QC3CONTROL_H_

#define QC3_DP_PIN 1
#define QC3_DM_PIN 0

#define QC3_MIN_VOLTAGE_MV 3600
#define QC3_CLASS_A_MAX_VOLTAGE_MV 12000
#define QC3_CLASS_B_MAX_VOLTAGE_MV 20000

// timing values for Portable Device are not available, indicative values for a HVDCP charger were taken from the uP7104 data sheet https://www.upi-semi.com/files/1889/1b8dae21-e91a-11e6-97d5-f1910565ec6d
#define QC_T_GLITCH_BC_DONE_MS 1500
#define QC_T_GLICH_V_CHANGE_MS 60
#define QC_T_ACTIVE_MS 1
#define QC_T_INACTIVE_MS 1

/**
    *  @brief Starts the handshake with the QC source, specifying whether the source is "class B" compliant (up to 20V) or not.
    *  
    *  @details A handshake is needed to be able to set the voltage. 
    *  begin() may be left out, in which case the first method modifying the voltage will silently call begin(false).
    *
    *  begin() is **blocking code**. It waits for a fixed period counting from the start up of the Arduino to act because the handshake needs a minimum time. But this is most likely not a problem because if you need 9V or 12V in your application, there is no gain in proceeding when the voltage isn't there yet (because of the handshake). And by putting begin() (or a call to one of the setXXX() functions) at the end of setup() (or other initialization) you can even do stuff while waiting because it counts from Arduino startup.
    *  
    *  @see begin(), setMilliVoltage(uint16_t), set5V(), set9V(), set12V(), set20V()
	*  @warning QC3_begin MUST be called before anything else
    */
void QC3_begin(uint8_t classB);

/**
    *  @brief Sets the desired voltage of the QC3.0 source.
    *  
    *  @details Will always set the passed voltage using continuous (QC3) mode (even for 5V, 9V, 12V and 20V). 
    *  To force usage of discrete (QC2) mode, please use set5V(), set9V(), set12V() or set20V();
    *
    *  @note If no handshake has been done (via begin()) with the QC source, the first call to setMilliVoltage() will result in a call to begin() to do the handshake.
    *  @note Setting an unreachable voltage will result in the closest supported voltage being set.
    *  @note Calling this method on a QC2 charger will not work.
    *  
    *  @see set5V(), set9V(), set12V()
    *
    *  @param [in] milliVolt The desired voltage in mV (between 3600mV and 12000mV).
    *  
    */
void QC3_setMilliVoltage(uint16_t milliVolt);

/**
    *  @brief Return the voltage that the charger is supposed to currently provide.
    *  
    *  @details Same as getVoltage, but returns an integer value in milliVolts
    *  
    *  @note The library has no notion if the voltage is really set. It just tries to set it but if you just start it all of a QC3.0 source it should set the correct voltage.
    *  
    *  @see getVoltage()
    *  
    *  @return The voltage that the charger is supposed to currently provide, in milliVolt
    */
uint16_t QC3_getMilliVoltage();

/**
    *  @brief Set voltage to 5V
    *  
    *  @details Sets the output of the QC source to 5V using discrete (QC2) mode.
    *  
    *  @note If no handshake has been done (via begin()) with the QC source, the first call to set5V() will result in a call to begin() to do the handshake.
    */
void QC3_set5V();

/**
    *  @brief Set voltage to 9V
    *  
    *  @details Sets the output of the QC source to 9V using discrete (QC2) mode.
    *  
    *  @note If no handshake has been done (via begin()) with the QC source, the first call to set9V() will result in a call to begin() to do the handshake.
    */
void QC3_set9V();

/**
    *  @brief Set voltage to 12V
    *  
    *  @details Sets the output of the QC source to 12V using discrete (QC2) mode.
    *  
    *  @note If no handshake has been done (via begin()) with the QC source, the first call to set12V() will result in a call to begin() to do the handshake.
    */
void QC3_set12V();

/**
    *  @brief Set voltage to 20V
    *  
    *  @details Sets the output of the QC Class B source to 20V using discrete mode.
    *  
    *  @note If no handshake has been done (via begin()) with the QC source, the first call to set20V() will result in a call to begin() to do the handshake.
    */
void QC3_set20V();

/**
    *  @brief Increment the desired voltage of the QC3.0 source by 200mV.
    *  
    *  @details Will request an increment of the voltage by 200mV. Performing the increment request when the maximum value is already reached has no effect.
    *  
    *  @note If no handshake has been done (via begin()) with the QC source, the first call to incrementVoltage() will result in a call to begin() to do the handshake, then the voltage will be incremented starting from 5V
    */
void QC3_incrementVoltage();

/**
    *  @brief Decrement the desired voltage of the QC3.0 source by 200mV.
    *  
    *  @details Will request a decrement of the voltage by 200mV. Performing the decrement request when the minimum value is already reached has no effect.
    *  
    *  @note If no handshake has been done (via begin()) with the QC source, the first call to decrementVoltage() will result in a call to begin() to do the handshake, then the voltage will be decremented starting from 5V
    */
void QC3_decrementVoltage();

/**
 * @brief Return whether the library is running in continuous (QC3) mode
 */
uint8_t QC3_isContinuous();

#endif /* QC3CONTROL_H_ */