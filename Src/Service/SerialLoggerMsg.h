//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: MSG_SERVICESTARTED
//
// MessageText:
//
//  The service has started.
//  %nCapturing port %1 to %2.
//
#define MSG_SERVICESTARTED               0x00000001L

//
// MessageId: MSG_SERVICESTOPPED
//
// MessageText:
//
//  The service has stopped.
//
#define MSG_SERVICESTOPPED               0x00000002L

//
// MessageId: MSG_SERVICEPAUSED
//
// MessageText:
//
//  The service is paused.
//
#define MSG_SERVICEPAUSED                0x00000003L

//
// MessageId: MSG_SERVICECONTINUED
//
// MessageText:
//
//  The service has continued.
//
#define MSG_SERVICECONTINUED             0x00000004L

//
// MessageId: MSG_TIMEOUT
//
// MessageText:
//
//  The service has not received data for %1 seconds.
//
#define MSG_TIMEOUT                      0x00000005L

//
// MessageId: MSG_GENERICINFO
//
// MessageText:
//
//  %1
//
#define MSG_GENERICINFO                  0x000003E8L

//
// MessageId: MSG_GENERICERROR
//
// MessageText:
//
//  %1
//
#define MSG_GENERICERROR                 0x000003E9L

//
// MessageId: MSG_ASSERTFAILED
//
// MessageText:
//
//  Assertion failed: expression %1 in file %2 on line %3.
//  %nGetLastError() is %4.
//
#define MSG_ASSERTFAILED                 0x000003EAL

//
// MessageId: MSG_ASSERTFAILED2
//
// MessageText:
//
//  Assertion failed: expression %1 in file %2 on line %3.
//  %nGetLastError() is %4.
//  %n%5 = %6.
//
#define MSG_ASSERTFAILED2                0x000003EBL

