#include <wiringPi.h>
#include <softPwm.h>

#define LED   1U

int main (void)
{
    int i;
    unsigned int duty = 0;

    //wiringPiSetupGpio() ;
    wiringPiSetup();

    pinMode(LED, OUTPUT) ;

    for (i = 0; i < 5; i++) {

        if (i & 0x1) {
            digitalWrite(LED, HIGH);
        } else {
            digitalWrite(LED, LOW);
        }

        delay(1000);

    }

    pinMode(LED, SOFT_PWM_OUTPUT) ;
    softPwmCreate(LED, 0, 100); /* range 0 - 100 */

    while (1) {
        softPwmWrite(LED, (duty++) % 100);
        delay(30);

#if 0
        softPwmWrite(LED, 0);
        delay(100);

        softPwmWrite(LED, 50);
        delay(100);

        softPwmWrite(LED, 100);
        delay(100);

        //delayMicroseconds(1000);
#endif
    }
    return 0 ;
}
