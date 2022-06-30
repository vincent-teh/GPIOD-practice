#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#ifndef CONSUMER
#define CONSUMER    "Consumer"
#endif

void *blinking_led_function1( void *ptr );
void *blinking_led_function2( void *ptr );

int main(int argc, char const *argv[])
{
    pthread_t thread1, thread2;
    struct gpiod_chip *chip;
	struct gpiod_line *line1;
    struct gpiod_line *line2;
    int line_number1 = 0;
    int line_number2 = 2;
    int ret;
    int iret1, iret2;

    chip = gpiod_chip_open_by_number(0);
    line1 = gpiod_chip_get_line(chip, line_number1);
    line2 = gpiod_chip_get_line(chip, line_number2);
    ret = gpiod_line_request_output(line1, CONSUMER, 0);
    ret = gpiod_line_request_output(line2, CONSUMER, 0);

    iret1 = pthread_create( &thread1, NULL, blinking_led_function1, (void*) line1);
    iret2 = pthread_create( &thread2, NULL, blinking_led_function2, (void*) line2);

    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);

release_line:
    gpiod_line_release(line1);
    gpiod_line_release(line2);

close_chip:
    gpiod_chip_close(chip);

ending:
    return 0;
}

void *blinking_led_function1( void *ptr ){
    int i, ret, val = 0;
    struct gpiod_line *line = ptr;

    for (i = 20; i > 0; i--) {
        ret = gpiod_line_set_value(line, val);
        sleep(1);
        val=!val;
    }
}

void *blinking_led_function2( void *ptr ){
    int i, ret, val = 0;
    struct gpiod_line *line = ptr;

    for (i = 20; i > 0; i--) {
        ret = gpiod_line_set_value(line, val);
        usleep(900000);
        val=!val;
    }
}
