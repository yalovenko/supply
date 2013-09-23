
#include "i2c_slave_ctrl.h"
#include "hal.h"

#include "hdw_cfg.h"
#include "pawn_ctrl.h"

static uint8_t outBuffer[ I2C_OUT_BUFFER_SZ ];
static uint8_t inBuffer[ I2C_IN_BUFFER_SZ ];
//static Mutex   mutex;

static InputQueue inputQueue;
#define QUEUE_SZ (I2C_IN_BUFFER_SZ * EXEC_QUEUE_SIZE)
static uint8_t queue[ I2C_IN_BUFFER_SZ * EXEC_QUEUE_SIZE ];

static const I2CConfig i2cfg1 =
{
    OPMODE_I2C,
    100000,
    STD_DUTY_CYCLE,
};

static void i2cRxCb( I2CDriver * i2cp );
static void i2cTxCb( I2CDriver * i2cp );

static WORKING_AREA( waExec, 1024 );
static msg_t execThread( void *arg );



void initI2cSlave( void )
{
    //palSetPadMode( GPIOB, 6, PAL_MODE_STM32_ALTERNATE_OPENDRAIN );
    //palSetPadMode( GPIOB, 7, PAL_MODE_STM32_ALTERNATE_OPENDRAIN );
    palSetPadMode( GPIOB, 6, PAL_MODE_INPUT );
    palSetPadMode( GPIOB, 7, PAL_MODE_INPUT );

    // Initialize mailbox.
    chIQInit( &inputQueue, queue, I2C_IN_BUFFER_SZ * EXEC_QUEUE_SIZE, NULL );
    // Creating thread.
    chThdCreateStatic( waExec, sizeof(waExec), NORMALPRIO, execThread, NULL );
}

int setI2cSlaveEn( uint8_t en, uint8_t addr )
{
	if ( en )
	{
		static msg_t status;
		static systime_t tmo;
		tmo = MS2ST( I2C_TIMEOUT );

		palSetPadMode( GPIOB, 6, PAL_MODE_STM32_ALTERNATE_OPENDRAIN );
		palSetPadMode( GPIOB, 7, PAL_MODE_STM32_ALTERNATE_OPENDRAIN );
		while ( 1 )
		{
			i2cStart( &I2CD1, &i2cfg1 );

			addr = ( addr != 0 ) ? addr : I2C_ADDRESS;
			status = i2cSlaveIoTimeout( &I2CD1,    addr,
										inBuffer,  I2C_IN_BUFFER_SZ,
										outBuffer, I2C_OUT_BUFFER_SZ,
										i2cRxCb,
										i2cTxCb,
										tmo );
			if ( status )
			{
				i2cStop( &I2CD1 );
				return status;
			}
			return 0;
		}
	}
	else
	{
	    i2cStop( &I2CD1 );
	    palSetPadMode( GPIOB, 6, PAL_MODE_INPUT );
	    palSetPadMode( GPIOB, 7, PAL_MODE_INPUT );
	}
}

static void i2cRxCb( I2CDriver * i2cp )
{
    (void)i2cp;
    // Command processing.
    chSysLockFromIsr();
        // Make CPU know command is not processed yet.
        outBuffer[0] = 0;
        // Send command to processing thread.
        static uint32_t i;
        static msg_t res;
        for ( i=0; i<I2C_IN_BUFFER_SZ; i++ )
        {
            res = chIQPutI( &inputQueue, inBuffer[i] );
            if ( res != Q_OK )
                break;
        }
    chSysUnlockFromIsr();
}

static void i2cTxCb( I2CDriver * i2cp )
{
    (void)i2cp;
    // Refresh buffer if necessary.
    //palTogglePad( GPIOB, 11 );
}

static msg_t execThread( void *arg )
{
    (void)arg;
    chRegSetThreadName( "e" );
    while ( 1 )
    {
        static size_t sz;
        static uint8_t buffer[ I2C_IN_BUFFER_SZ ];
        sz = chIQReadTimeout( &inputQueue, buffer, I2C_IN_BUFFER_SZ, TIME_INFINITE );

        static uint32_t uvalue32Out;
        static uint16_t uvalue16Out;
        static uint8_t  uvalue8Out;

        static uint16_t * puvalue16In;
        static int i;


        puvalue16In = (uint16_t *)(&buffer[1]);
        // Parse inBuffer
        switch ( buffer[0] )
        {
        /*
        case CMD_SET_POWER_TIMEOUT:
            setPowerTimeout( puvalue16In[0] );
            break;
        case CMD_POWER_RESET:
            powerOffReset();
            break;
        case CMD_SHUTDOWN_RESET:
            powerOffReset();
            break;
        case CMD_SET_SECONDS_PER_DAY:
            setSecondsPerDay( buffer[1] + (buffer[2] << 8) + (buffer[3] << 16) + (buffer[4] << 24) );
            break;
        case CMD_SET_CURRENT_TIME:
            setCurrentTime( buffer[1] + (buffer[2] << 8) + (buffer[3] << 16) + (buffer[4] << 24) );
            break;
        case CMD_CURRENT_TIME:
            uvalue32Out = currentTime();
            outBuffer[1] = uvalue32Out & 255;
            outBuffer[2] = (uvalue32Out >> 8) & 255;
            outBuffer[3] = (uvalue32Out >> 16) & 255;
            outBuffer[4] = (uvalue32Out >> 24) & 255;
            outBuffer[0] = CMD_CURRENT_TIME;
            break;
        case CMD_SET_WAKEUPS_CNT:
            setWakeupsCnt( buffer[1] );
            break;
        case CMD_SET_WAKEUP:
            setWakeup( buffer[1], buffer[2] + (buffer[3] >> 8) + (buffer[4] >> 16) + (buffer[5] >> 24) );
            break;
        case CMD_TEMP:
            uvalue16Out = adcTemperature();
            outBuffer[1] = (uint8_t)(uvalue16Out & 0xFF);
            outBuffer[2] = (uint8_t)(uvalue16Out >> 8);
            outBuffer[0]  = CMD_TEMP;
            break;
        case CMD_CURR:
            uvalue16Out = adcCurrent();
            outBuffer[1] = (uint8_t)(uvalue16Out & 0xFF);
            outBuffer[2] = (uint8_t)(uvalue16Out >> 8);
            outBuffer[0]  = CMD_CURR;
            break;
        case CMD_BUCK_FB:
            uvalue16Out = adcBuckFb();
            outBuffer[1] = (uint8_t)(uvalue16Out & 0xFF);
            outBuffer[2] = (uint8_t)(uvalue16Out >> 8);
            outBuffer[0]  = CMD_BUCK_FB;
            break;
        case CMD_BOOST_FB:
            uvalue16Out = adcBoostFb();
            outBuffer[1] = (uint8_t)(uvalue16Out & 0xFF);
            outBuffer[2] = (uint8_t)(uvalue16Out >> 8);
            outBuffer[0]  = CMD_BOOST_FB;
            break;
        case CMD_SOLAR_FB:
            uvalue16Out = adcSolarFb();
            outBuffer[1] = (uint8_t)(uvalue16Out & 0xFF);
            outBuffer[2] = (uint8_t)(uvalue16Out >> 8);
            outBuffer[0]  = CMD_SOLAR_FB;
            break;
        case CMD_SET_SOLAR_VOLT:
            convSetSolar( *puvalue16In );
            break;
        case CMD_SET_CHARGE_VOLT:
            convSetBoost( *puvalue16In );
            break;
        case CMD_SET_MOTO_EN:
            motoSetEn( *puvalue16In );
            break;
        case CMD_SET_MOTO:
            motoSet( *puvalue16In );
            break;
        case CMD_SET_LIGHT:
            setLight( *puvalue16In );
            break;
        case CMD_SET_LED:
            setLed( *puvalue16In );
            break;
        // Pawn commands.
        case CMD_SETUP_OSC:
            oscSetup( *puvalue16In, buffer[3] );
            break;
        case CMD_OSC_STATUS:
            uvalue16Out = oscMeasuresCnt( &uvalue8Out );
            outBuffer[1] = (uint8_t)(uvalue16Out & 0xFF);
            outBuffer[2] = (uint8_t)(uvalue16Out >> 8);
            outBuffer[3] = uvalue8Out;
            outBuffer[0] = CMD_OSC_STATUS;
            break;
        case CMD_OSC:
            uvalue16Out = oscValue( *puvalue16In );
            outBuffer[1] = (uint8_t)(uvalue16Out & 0xFF);
            outBuffer[2] = (uint8_t)(uvalue16Out >> 8);
            outBuffer[0]  = CMD_OSC;
            break;
        */

        case I2C_CMD_PAWN_SET_IO:
            pawnSetIo( buffer[1], &buffer[2] );
            outBuffer[0] = I2C_CMD_PAWN_SET_IO;
            break;
        case I2C_CMD_PAWN_IO:
        	pawnIo( buffer[1], &outBuffer[1] );
        	outBuffer[0] = I2C_CMD_PAWN_IO;
        	break;
        case I2C_CMD_PAWN_SET_MEM:
        	uvalue8Out = buffer[1];
        	uvalue16Out = (uint16_t)buffer[2] + ( ((uint16_t)buffer[3]) << 8 );
        	pawnSetMem( uvalue8Out, uvalue16Out, &buffer[4] );
        	outBuffer[0] = I2C_CMD_PAWN_SET_MEM;
        	break;
        case I2C_CMD_PAWN_WRITE_FLASH:
        	puvalue16In = (uint16_t *)(&buffer[1]);
        	uvalue16Out = pawnWriteFlash( puvalue16In[0] );
        	outBuffer[1] = (uint8_t)(uvalue16Out & 0xFF);
        	outBuffer[2] = (uint8_t)(uvalue16Out >> 8);
        	outBuffer[0] = I2C_CMD_PAWN_WRITE_FLASH;
        	break;
        case I2C_CMD_PAWN_RUN:
        	pawnRun();
        	outBuffer[0] = I2C_CMD_PAWN_RUN;
        	break;
        case I2C_CMD_PAWN_IS_RUNNING:
        	uvalue8Out = pawnIsRunning();
        	outBuffer[1] = uvalue8Out;
        	outBuffer[0] = I2C_CMD_PAWN_IS_RUNNING;
        	break;
        case I2C_CMD_PAWN_STOP:
        	pawnStop();
        	outBuffer[0] = I2C_CMD_PAWN_STOP;
        	break;
        case I2C_CMD_PAWN_RESULT:
        	i = pawnResult();
        	outBuffer[1] = (uint8_t)( i & 0xFF );
        	outBuffer[2] = (uint8_t)( (i >> 8) & 0xFF );
        	outBuffer[3] = (uint8_t)( (i >> 16) & 0xFF );
        	outBuffer[4] = (uint8_t)( (i >> 24) & 0xFF );
        	outBuffer[0] = I2C_CMD_PAWN_RESULT;
        case I2C_CMD_PAWN_ERROR:
        	i = pawnError();
        	outBuffer[1] = (uint8_t)( i & 0xFF );
        	outBuffer[2] = (uint8_t)( (i >> 8) & 0xFF );
        	outBuffer[3] = (uint8_t)( (i >> 16) & 0xFF );
        	outBuffer[4] = (uint8_t)( (i >> 24) & 0xFF );
        	outBuffer[0] = I2C_CMD_PAWN_ERROR;
            break;

        }
    }
    return 0;
}




int setI2cEn( uint8_t en )
{

	return 0;
}

int i2cIo( uint8_t addr,
		   uint8_t * outBuffer, int outSz,
		   uint8_t * inBuffer,  int inSz, int timeoutMs )
{
	int status;
	if ( outSz > 0 )
	{

	}
	else
	{

	}
}







