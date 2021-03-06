
#include "mcuctrl.h"

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define I2C_CMD_VERSION          1
#define I2C_CMD_PAWN_SET_IO      2
#define I2C_CMD_PAWN_IO          3
#define I2C_CMD_PAWN_SET_MEM     4
#define I2C_CMD_PAWN_WRITE_FLASH 5
#define I2C_CMD_PAWN_RUN         6
#define I2C_CMD_PAWN_IS_RUNNING  7
#define I2C_CMD_PAWN_STOP        8
#define I2C_CMD_PAWN_RESULT      9
#define I2C_CMD_PAWN_ERROR       10


class McuCtrl::PD
{
public:
    PD() {}
    ~PD() {}

    int fd;

    static const int SUPPLY_ADDRESS;
    static const int DELAY;
    static const int TRIES_CNT;
};

const int McuCtrl::PD::SUPPLY_ADDRESS = 1;
const int McuCtrl::PD::DELAY          = 5;
const int McuCtrl::PD::TRIES_CNT      = 100;

McuCtrl::McuCtrl()
{
    pd = new PD();
    pd->fd = -1;
}

McuCtrl::~McuCtrl()
{
    if ( isOpen() )
        close();
    delete pd;
}

bool McuCtrl::open()
{
    const char fileName[] = "/dev/i2c-0";   // Name of the port we will be using
    pd->fd = ::open(fileName, O_RDWR);
    return ( pd->fd >= 0 );
}

bool McuCtrl::isOpen()
{
    return ( pd->fd >= 0 );
}

void McuCtrl::close()
{
    ::close( pd->fd );
    pd->fd = -1;
}

bool McuCtrl::write( unsigned char addr, const unsigned char * data, int cnt )
{
    if ( ioctl( pd->fd, I2C_SLAVE, addr ) < 0 )
        return false;
    int sz = ::write( pd->fd, data, cnt );
    return ( sz == cnt );
}

bool McuCtrl::read( unsigned char addr, unsigned char * data, int cnt )
{
    if ( ioctl( pd->fd, I2C_SLAVE, addr ) < 0 )
        return false;
    int sz = ::read( pd->fd, data, cnt );
    return ( sz == cnt );
}

bool McuCtrl::setIo( int index, unsigned char value )
{
    if ( !ensureOpen() )
        return false;
    unsigned char data[3];
    data[0] = I2C_CMD_PAWN_SET_IO;
    data[1] = static_cast<unsigned char>( index );
    data[2] = value;
    return McuCtrl::write( PD::SUPPLY_ADDRESS, data, sizeof(data) );
}

bool McuCtrl::io( int index, unsigned char & value )
{
    if ( !ensureOpen() )
        return false;
    unsigned char data[3];
    data[0] = I2C_CMD_PAWN_IO;
    data[1] = static_cast<unsigned char>( index );
    data[2] = 0;
    bool res = McuCtrl::write( PD::SUPPLY_ADDRESS, data, sizeof(data) );
    // Several attempts to get data.
    for ( int i=0; i<PD::TRIES_CNT; i++ )
    {
        res = McuCtrl::read( PD::SUPPLY_ADDRESS, data, sizeof(data) );
        if ( !res )
            return false;
        if ( data[0] == I2C_CMD_PAWN_IO )
            break;
        usleep( 1000 * PD::DELAY );
    }
    if ( data[0] != I2C_CMD_PAWN_IO )
        return false;
    value = static_cast<int>( data[1] );
    return true;
}

bool McuCtrl::reopen()
{
    McuCtrl::close();
    return McuCtrl::open();
}

bool McuCtrl::ensureOpen()
{
    if ( McuCtrl::isOpen() )
        return true;
    return McuCtrl::open();
}





