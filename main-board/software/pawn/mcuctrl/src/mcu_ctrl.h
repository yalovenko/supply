
#ifndef __MCU_CTRL_H_
#define __MCU_CTRL_H_

#include "usb_io.h"

class McuCtrl: public UsbIo
{
public:
    McuCtrl();
    ~McuCtrl();

    bool flash( const std::string & fileName, std::string & result );
    bool start();
    bool stop();
    bool setIo( const std::basic_string<unsigned char> & args );
    bool io( int cnt, std::basic_string<unsigned char> & args );

    /*
    bool inputs( unsigned long * data, int len = 2 );
    bool setOutputs( unsigned long * data, int len = 2 );

    bool accInit();
    bool accAcc( int & x, int & y, int & z );
    bool accMag( int & x, int & y, int & z );
    bool accTemp( int & t );

    bool i2cSetAddr( int addr );
    bool i2cSetBuf( int start, unsigned char * data, int cnt );
    bool i2cIo( int txCnt, int rxCnt );
    bool i2cStatus( int & status );
    bool i2cBuffer( int cnt, unsigned char * data );
    */
};


#endif





