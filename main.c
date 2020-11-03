////////////////////////////////////////////////////////////////////////////////
///
/// Simple moving average algorithm. Averages are typically done when during
/// measurements to introduce less bounce and stability in the readings. This
/// It depending upon the sensitivity of the calculations or measurement
/// different average lengths can be used for same measurements.  In embedded
/// systems measurements can be light, pressure, temperature, tachometer.
/// Measurements are done by A-D and then calculations are then done.
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// include section
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint-gcc.h>

////////////////////////////////////////////////////////////////////////////////
/// define section
////////////////////////////////////////////////////////////////////////////////
#define SAMPLES 10  ///
#define OUTPUT printf /// make output screen for now

////////////////////////////////////////////////////////////////////////////////
/// Data Structures Section
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    uint16_t  *sample;  /// sample array
    uint16_t  Asize;     /// array size
    uint16_t  Acnt;      /// sample count
} SMAvg_s;

////////////////////////////////////////////////////////////////////////////////
/// Globals
////////////////////////////////////////////////////////////////////////////////
static  SMAvg_s  *AvgMea1;   /// Simple moving average data structure

////////////////////////////////////////////////////////////////////////////////
/// Functions Section
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Handler 1
/// @param error line number
/// @param error file
/// @return abort()
////////////////////////////////////////////////////////////////////////////////
/// Values of error numbers below so that cross referencing is not needed.
////////////////////////////////////////////////////////////////////////////////
/// errno value       Error
/// 1             /* Operation not permitted */
/// 2             /* No such file or directory */
/// 3             /* No such process */
/// 4             /* Interrupted system call */
/// 5             /* I/O error */
/// 6             /* No such device or address */
/// 7             /* Argument list too long */
/// 8             /* Exec format error */
/// 9             /* Bad file number */
/// 10            /* No child processes */
/// 11            /* Try again */
/// 12            /* Out of memory */
/// 13            /* Permission denied */
////////////////////////////////////////////////////////////////////////////////
void Handler1( uint32_t ln, char *fn )
{
    OUTPUT("Error file: %s line: %i\n\n", fn, ln);
    if ( errno > 0 )
    {
        OUTPUT("Value of errno: %d\n", errno);
        OUTPUT("Printed message : %s\n", strerror(errno));
    }
    abort();
}


////////////////////////////////////////////////////////////////////////////////
/// Initialize data structure used for simple moving average
/// @param  size size of data set
/// @return  data structure used calculate simple moving average.
////////////////////////////////////////////////////////////////////////////////
SMAvg_s *SMAvgInit(int size)
{
    SMAvg_s *SMAData = NULL;

    SMAData = malloc(sizeof(SMAvg_s));

    if ( NULL == SMAData )
    {
        Handler1(__LINE__,__FILE__);
    }

    SMAData->sample = calloc(size, sizeof(int));

    if (   NULL == SMAData->sample )
    {
        Handler1(__LINE__,__FILE__);
    }

    SMAData->Asize = size;
    SMAData->Acnt = 0;

    return (SMAData);
}


////////////////////////////////////////////////////////////////////////////////
/// Simple moving average
///
/// @param  ArSample-> Simple moving averaging data-structure
/// @param  sample-> Data to be added to averaging
/// @return Simple moving average
////////////////////////////////////////////////////////////////////////////////
double SimMovAvg(SMAvg_s *ArSample, int sdata)
{
    double arsum = 0.0;
    int arsz = 0;

    ArSample->sample[((ArSample->Acnt) % ArSample->Asize)] = sdata;
    ++(ArSample->Acnt);

    /// sample data in circular buffer
    arsz = (ArSample->Acnt < ArSample->Asize) ? ArSample->Acnt : ArSample->Asize;

    for (int i=0; (i < arsz); i++)
    {
        arsum = arsum + ArSample->sample[i];
    }

    return ( arsum / arsz );
}


////////////////////////////////////////////////////////////////////////////////
/// Initialize all pointers used for measurement averages
/// @param Uses data structure
/// @retun no return
////////////////////////////////////////////////////////////////////////////////
void InitSMA(void)
{
    AvgMea1 = SMAvgInit(SAMPLES);
}


////////////////////////////////////////////////////////////////////////////////
/// Test simple moving averages for sanity
/// Quick sanity check
/// @param none
/// @return none
////////////////////////////////////////////////////////////////////////////////
void Test1(void)
{
    double ans = 0;
    for(uint8_t i=0; ( i < SAMPLES ); i++ )
    {
        ans = SimMovAvg(AvgMea1,25);
    }

    if ( (double) 25 == ans )
    {
       OUTPUT("Test1 : Pass\n");
    }
    else
    {
        OUTPUT("Test1 : Fail\n");
    }

    return;
}


////////////////////////////////////////////////////////////////////////////////
/// Test simple moving averages
/// Value check
/// @param none
/// @return none
////////////////////////////////////////////////////////////////////////////////
void Test2(void)
{
    double ans = 0;
    uint16_t measurement[10] =  {10,11,12,13,14,15,16,17,18,19};
    for(uint8_t i=0; ( i < SAMPLES ); i++ )
    {
        ans = SimMovAvg(AvgMea1,measurement[i]);
    }

    if ( (double) 14.5 == ans )
    {
       OUTPUT("Test2 : Pass\n");
    }
    else
    {
        OUTPUT("Test2 : Fail\n");
    }

    return;
}


////////////////////////////////////////////////////////////////////////////////
/// Test error handle1 check
/// Value check
/// @param none
/// @return none
////////////////////////////////////////////////////////////////////////////////
void Test3(void)
{
    Handler1(__LINE__,__FILE__);
    return;
}


////////////////////////////////////////////////////////////////////////////////
/// Main
/// @param none
////////////////////////////////////////////////////////////////////////////////
int main()
{

    InitSMA();

    Test1();
    Test2();
    Test3();


    return 0;
}
