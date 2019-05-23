/**
 * Easy Object Dictionary
 *
 * @brief   Defines and Handles all data structure types for communication over e.g. EasyProtocol
 *
 * @version Jul 27, 2016 - Release Version.
 *          Jun 20, 2017 - Add Ep_Calib data set. Release Version.
 *          Jul 10, 2017 - Add EP_CMD_SETTING_ dsata set. Release Version.
 *          Feb 22, 2018 - Add Ep_Status_SysState for reading QoS. Release Version.
 *          Mar 06, 2018 - Add support for reading Ep_Id.
 *                         Add support for version self-adapt for Ep_Setting. All relevant changes
 *                         are done within the EasyObjectDictionary.h and EasyObjectionary.cpp files,
 *                         which can be found using the '[Legacy Support]' keyword.
 *          Aug 02, 2018 - Update comments.
 *
 * @attention
 *          *****        DO NOT CHANGE THIS FILE           *****
 *          ***** Automatically Generated by IMU Assistant *****
 *
 *
 * @attention
 * <h2><center>&copy; COPYRIGHT(c) 2018 SYD Dynamics ApS</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef EASYOBJECTDICTIONARY_H
#define EASYOBJECTDICTIONARY_H

#include "BasicTypes.h"


//--------------------------------------------------------------
// Header
#define EP_CMD_TYPE_          uint8
#define EP_CMD_BITS_            (7)
#define EP_CMD_MASK_  (0x0000007fu)
#define EP_RES_TYPE_          uint8
#define EP_RES_BITS_            (3)
#define EP_RES_MASK_  (0x00000007u)
#define EP_ID_TYPE_          uint16
#define EP_ID_BITS_            (11)
#define EP_ID_MASK_   (0x000007ffu)

typedef struct{
    uint32       cmd : EP_CMD_BITS_;      // Command Identifier (e.g. EP_CMD_RPY_ is one of the possible Command Identifier).
    uint32       res : EP_RES_BITS_;      // Reserved Bits. Must set to Zero.
    uint32    fromId : EP_ID_BITS_;       // The Device Short-ID indicating the source of the data being delivered.
    uint32      toId : EP_ID_BITS_;       // The Device Short-ID indicating the destination of the data being delivered.
} Ep_Header;
// Header
//--------------------------------------------------------------


//--------------------------------------------------------------
// Common ID Definition
#define EP_ID_BROADCAST_          ((EP_ID_TYPE_) 0x0000)
#define EP_ID_UNSPECIFIED_        ((EP_ID_TYPE_) 0x0001)
#define EP_ID_HOST_               ((EP_ID_TYPE_) 0x0002)  // HOST refers to PCs or similar
                                                          //   e.g. the Host is the PC running ImuAssistant
// Common ID Definition
//--------------------------------------------------------------


#define global_SysShortId         (EP_ID_HOST_)           // Set the current device to HOST
#define global_SysQoS             (0)                     // 0 means QoS not available.


//--------------------------------------------------------------
// Command Identifier Definition
#define EP_CMD_REQUEST_           ((EP_CMD_TYPE_)12)
#define EP_CMD_ACK_               ((EP_CMD_TYPE_)13)

#define EP_CMD_STATUS_            ((EP_CMD_TYPE_)22)
#define EP_CMD_CALIB_             ((EP_CMD_TYPE_)23)

#define EP_CMD_Q_S1_S_            ((EP_CMD_TYPE_)31)
#define EP_CMD_Q_S1_E_            ((EP_CMD_TYPE_)32)
#define EP_CMD_EULER_S1_S_        ((EP_CMD_TYPE_)33)
#define EP_CMD_EULER_S1_E_        ((EP_CMD_TYPE_)34)
#define EP_CMD_RPY_               ((EP_CMD_TYPE_)35)
#define EP_CMD_GRAVITY_           ((EP_CMD_TYPE_)36)

#define EP_CMD_Raw_GYRO_ACC_MAG_  ((EP_CMD_TYPE_)41)
// Command Identifier Definition
//--------------------------------------------------------------


typedef struct{
    Ep_Header         header;
    EP_CMD_TYPE_      cmdAck;      // The command it acknowledges to
} Ep_Ack;

typedef struct{
    Ep_Header         header;
    EP_CMD_TYPE_  cmdRequest;      // The command requested
} Ep_Request;

typedef union{
    uint16 all_Bits;
    struct{
        uint16 qos  : 3;  // Quality of Service.
                          // 0: Service unavailable due to boot or in sleep mode.
                          // 1: Service unavailable due to System Fault.
                          // 2: Limited Service -- Some functions are not available / Very limited measurement accuracy
                          //                    (e.g. After Dynamic Boot)
                          // 3: Basic Service   -- All functions available and providing basic performance.
                          //                    (e.g. After Static Boot)
                          // 4: Fine Service    -- All function available and providing fine performance.
                          //                    (e.g. DynamicGyroCalib success 30 seconds after boot)
                          // 5: Very Good Service -- All function available and providing very good performance.
                          //                    (e.g. Two DynamicGyroCalib success after 5 minutes)
                          // 6: Reserved.
                          // 7: Use extended QoS definition.
        uint16      :13;  // Unused Bits
    }bits;
} Ep_Status_SysState;

typedef struct {
    Ep_Header            header;
    uint32            timeStamp;   // Timestamp              (Unit: uS)
    float32         temperature;   // Sensor temperature     (Unit: Celcius)
    uint16           updateRate;   // Internal sampling rate (Unit: Hz)
    Ep_Status_SysState sysState;   // System State
} Ep_Status;

typedef struct {
    Ep_Header      header;
    uint8       calibType;         // Use value 4 to run CalibB.
                                   // A received package with calibType==(4+0x10) means it is a status report of the CalibB.
                                   // Use value 5 to run Forced Calibration.
                                   // A received package with calibType==(5+0x10) means it is a status report of the Forced Calibration.

    uint8         ctrlVal;         // if(calibType==4)     ctrlVal=2   means to perform the calibration and the result will be saved into Flash Memory of the module.
                                   // if(calibType==4+0x10)ctrlVal=1   means CalibB successfully finished.
                                   // if(calibType==4+0x10)ctrlVal not equal to 1 or no response received, means CalibB failed.
                                   // if(calibType==5)     ctrlVal=1   means to reset the results of Forced Calibration.
                                   // if(calibType==5)     ctrlVal=2   means to perform the Forced Calibration.
                                   // if(calibType==5+0x10)ctrlVal=0   means Forced Calibration is processing.
                                   // if(calibType==5+0x10)ctrlVal=1   means Forced Calibration successfully finished.
                                   // if(calibType==5+0x10)ctrlVal=5   means previous Forced Calibration result is revoked successfully.
                                   // if(calibType==5+0x10)ctrlVal not equal to 1 nor 5, or no response received, means operation regarding Forced Calibration has failed.

    uint16         param1;         // if(calibType==4) Param1 should always be 0xaa35.
                                   // if(calibType==5) Param1 should always be 0xaa65.
    uint32         param2;         // Reserved.
    uint32         param3;         // Reserved.
} Ep_Calib;

typedef struct{
    Ep_Header      header;
    uint32      timeStamp;         // Timestamp when the data is sampled (Unit: uS)
    float32       gyro[3];         // Gyro raw data         (Unit: rad/s)
    float32        acc[3];         // Accel raw data        (Unit: g)
    float32        mag[3];         // Magnetometer raw data (Unit: one earth magnetic field)
} Ep_Raw_GyroAccMag;

typedef struct{
    Ep_Header      header;
    uint32      timeStamp;         // Timestamp when the Quaternion is calculated (Unit: uS)
    float32          q[4];         // Quaternion representing the rotation from the current sensor frame to the initial (power-on) sensor frame.
} Ep_Q_s1_s;

typedef struct{
    Ep_Header      header;
    uint32      timeStamp;         // Timestamp when the Quaternion is calculated (Unit: uS)
    float32          q[4];         // Quaternion representing the rotation from the current sensor frame to the Earth frame.
} Ep_Q_s1_e;

typedef struct{
    Ep_Header      header;
    uint32      timeStamp;         // Timestamp when the Euler angles are calculated (Unit: uS)
    float32           psi;         // Euler angle (psi)   from the current sensor frame to the initial (power-on) sensor frame (Unit: degree)
    float32         theta;         // Euler angle (theta) from the current sensor frame to the initial (power-on) sensor frame (Unit: degree)
    float32           phi;         // Euler angle (phi)   from the current sensor frame to the initial (power-on) sensor frame (Unit: degree)
} Ep_Euler_s1_s;

typedef struct{
    Ep_Header      header;
    uint32      timeStamp;         // Timestamp when the Euler angles are calculated (Unit: uS)
    float32           psi;         // Euler angle (psi)   from the current sensor frame to the Earth frame (Unit: degree)
    float32         theta;         // Euler angle (theta) from the current sensor frame to the Earth frame (Unit: degree)
    float32           phi;         // Euler angle (phi)   from the current sensor frame to the Earth frame (Unit: degree)
} Ep_Euler_s1_e;

typedef struct{
    Ep_Header      header;
    uint32      timeStamp;          // Timestamp when the Roll Pitch Yaw angles are calculated (Unit: uS)
    float32          roll;          // Roll                (Unit: degree)
    float32         pitch;          // Pitch               (Unit: degree)
    float32           yaw;          // Yaw (i.e. Heading)  (Unit: degree)
} Ep_RPY;

typedef struct{
    Ep_Header      header;
    uint32      timeStamp;          // Timestamp when the Gravity Vector is calculated (Unit: uS)
    float32          g[3];          // (g[0], g[1], g[2]) represents the vector of Earth Gravity in the sensor frame (Unit: g)
} Ep_Gravity;


#ifdef EP_PLATFORM_QT5_
Q_DECLARE_METATYPE(Ep_Header)
Q_DECLARE_METATYPE(Ep_Request)
Q_DECLARE_METATYPE(Ep_Ack)
Q_DECLARE_METATYPE(Ep_Status)
Q_DECLARE_METATYPE(Ep_Calib)
Q_DECLARE_METATYPE(Ep_Raw_GyroAccMag)
Q_DECLARE_METATYPE(Ep_Q_s1_s)
Q_DECLARE_METATYPE(Ep_Q_s1_e)
Q_DECLARE_METATYPE(Ep_Euler_s1_s)
Q_DECLARE_METATYPE(Ep_Euler_s1_e)
Q_DECLARE_METATYPE(Ep_RPY)
Q_DECLARE_METATYPE(Ep_Gravity)
#endif



//------------------------------------------------------------------------
// Object Items Database  (Part 1/2)

#define EOD_DB_SIZE_    (11)

typedef struct{
    EP_CMD_TYPE_         cmd;
    uint16              size;
} EOD_DB_Static;

typedef struct{
    void*               data;
    __IO uint8         mutex;
} EOD_DB_Dynamic;

#define EOD_MUTEX_LOCKED_        (0x1u)
#define EOD_MUTEX_UNLOCKED_      (0x2u)
#define EOD_MUTEX_WRITE_PROTECT_ (0x4u)
#define EOD_MUTEX_READ_PROTECT_  (0x8u)

    // The Dynamic DataBase:
#define EOD_DB_DYNAMIC_INIT    {\
    {&ep_Request,         EOD_MUTEX_UNLOCKED_      },\
    {&ep_Ack,             EOD_MUTEX_UNLOCKED_      },\
    {&ep_Status,          EOD_MUTEX_UNLOCKED_      },\
    {&ep_Calib,           EOD_MUTEX_UNLOCKED_      },\
    {&ep_Raw_GyroAccMag,  EOD_MUTEX_UNLOCKED_      },\
    {&ep_Q_s1_s,          EOD_MUTEX_UNLOCKED_      },\
    {&ep_Q_s1_e,          EOD_MUTEX_UNLOCKED_      },\
    {&ep_Euler_s1_s,      EOD_MUTEX_UNLOCKED_      },\
    {&ep_Euler_s1_e,      EOD_MUTEX_UNLOCKED_      },\
    {&ep_RPY,             EOD_MUTEX_UNLOCKED_      },\
    {&ep_Gravity,         EOD_MUTEX_UNLOCKED_      }\
}
// Object Items Database  (Part 2/2)
//------------------------------------------------------------------------



#ifdef EP_PLATFORM_QT5_
class EasyObjectDictionary : public QObject {
    Q_OBJECT
#else
class EasyObjectDictionary {
#endif
public:
    EasyObjectDictionary();
    ~EasyObjectDictionary();

//------------------------------------------------------------------------
// Basic Read & Write
public:
    // Whole Object:
    int Write(char* dataIn, int lengthIn, Ep_Header* headerOut);        // ByteArray -> OD
    int Read(EP_CMD_TYPE_ cmdIn, char** dataOut, int* lengthOut);       // ByteArray <= OD

    // Only Header:
    int Write_Header(EP_CMD_TYPE_ cmdIn, Ep_Header headerIn);           // headerIn  -> OD
    int Write_Header_toId(EP_CMD_TYPE_ cmdIn, EP_ID_TYPE_ toIdIn);      // toIdIn    -> OD
    int Read_Header(EP_CMD_TYPE_ cmdIn, Ep_Header* headerOut);          // headerOut <- OD
    int Read_Header(char* dataIn,  int lengthIn, Ep_Header* headerOut); // headerOut <- ByteArray
// Basic Read & Write 
//------------------------------------------------------------------------


// Maximum Size of Object Data:
public:
    int Get_MaxSize();
private:
    int maxSize;

// Object Data:
private:
    Ep_Request        ep_Request;
    Ep_Ack            ep_Ack;
    Ep_Status         ep_Status;
    Ep_Calib          ep_Calib;
    Ep_Raw_GyroAccMag ep_Raw_GyroAccMag;
    Ep_Q_s1_s         ep_Q_s1_s;
    Ep_Q_s1_e         ep_Q_s1_e;
    Ep_Euler_s1_s     ep_Euler_s1_s;
    Ep_Euler_s1_e     ep_Euler_s1_e;
    Ep_RPY            ep_RPY;
    Ep_Gravity        ep_Gravity;


//------------------------------------------------------------------------
// Advanced Read & Write
public:
    int Write_Ep_Ack(   EP_ID_TYPE_ toId, EP_CMD_TYPE_ cmdAck); 
    int Write_Ep_Status(EP_ID_TYPE_ toId, uint32 timeStamp, float32 temperature, uint16 updateRate);
    int Write_Ep_Calib( EP_ID_TYPE_ toId, uint8  calibType, uint8 ctrlVal, uint16 param1, uint32 param2, uint32 param3);
    int Write_Ep_Raw_GyroAccMag(EP_ID_TYPE_ toId, uint32 timeStamp,
                                float wx, float wy, float wz,
                                float ax, float ay, float az,
                                float mx, float my, float mz);
    int Write_Ep_Q_s1_s(    EP_ID_TYPE_ toId, uint32 timeStamp, float q1, float q2, float q3, float q4);
    int Write_Ep_Euler_s1_s(EP_ID_TYPE_ toId, uint32 timeStamp, float psi, float theta, float phi);
    int Write_Ep_Q_s1_e(    EP_ID_TYPE_ toId, uint32 timeStamp, float q1, float q2, float q3, float q4);
    int Write_Ep_Euler_s1_e(EP_ID_TYPE_ toId, uint32 timeStamp, float psi, float theta, float phi);
    int Write_Ep_RPY(       EP_ID_TYPE_ toId, uint32 timeStamp, float roll, float pitch, float yaw);
    int Write_Ep_Gravity(   EP_ID_TYPE_ toId, uint32 timeStamp, float gravityX, float gravityY, float gravityZ);
    int Write_Ep_Request(   EP_ID_TYPE_ toId, EP_CMD_TYPE_ cmdRequest);

    int Read_Ep_Request(    Ep_Request*    dataOut);
    int Read_Ep_Ack(        Ep_Ack*        dataOut);
    int Read_Ep_Status(     Ep_Status*     dataOut);
    int Read_Ep_Calib(      Ep_Calib*      dataOut);
    int Read_Ep_Raw_GyroAccMag(Ep_Raw_GyroAccMag* dataOut);
    int Read_Ep_Q_s1_s(     Ep_Q_s1_s*     dataOut);
    int Read_Ep_Euler_s1_s( Ep_Euler_s1_s* dataOut);
    int Read_Ep_Q_s1_e(     Ep_Q_s1_e*     dataOut);
    int Read_Ep_Euler_s1_e( Ep_Euler_s1_e* dataOut);
    int Read_Ep_RPY(        Ep_RPY*        dataOut);
    int Read_Ep_Gravity(    Ep_Gravity*    dataOut);
// Advanced Read & Write 
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// Object Items Database
public:
    int  EOD_DB_SetWriteProtect(EP_CMD_TYPE_ cmdIn, bool enable);
    int  EOD_DB_SetReadProtect(EP_CMD_TYPE_ cmdIn, bool enable);
protected:
    int  EOD_DB_FindKey(EP_CMD_TYPE_ cmdIn);
    EOD_DB_Dynamic   eOD_DB_Dynamic[ EOD_DB_SIZE_ ];
// Object Items Database
//------------------------------------------------------------------------

};


#endif // EASYOBJECTDICTIONARY_H
