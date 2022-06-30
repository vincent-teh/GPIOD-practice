#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#ifndef CONSUMER
#define CONSUMER    "Consumer"
#endif

void *blinking_led_function( void *ptr );

int main(int argc, char const *argv[])
{
    /*declaring variables*/
    pthread_t thread1, thread2;
    struct gpiod_chip *chip;
	struct gpiod_line *line1;   //led 1 @gpio0
    struct gpiod_line *line2;   //led 2 @gpio2
    struct gpiod_line *line3;   //interupt @gpio3
    struct gpiod_line_event event;
    struct timespec ts = { 1, 0 };

    int line_number1 = 0;
    int line_number2 = 2;
    int line_number3 = 3;
    int ret, i;
    int iret1, iret2;

    /*declaration proces*/
    chip = gpiod_chip_open_by_number(0);
    line1 = gpiod_chip_get_line(chip, line_number1);
    line2 = gpiod_chip_get_line(chip, line_number2);
    line3 = gpiod_chip_get_line(chip, line_number3);
    ret = gpiod_line_request_output(line1, CONSUMER, 0);
    ret = gpiod_line_request_output(line2, CONSUMER, 0);
    ret = gpiod_line_request_rising_edge_events(line3, CONSUMER);    //ask the gpiod to stop at rising edge

    /*function calling*/
    // iret1 = pthread_create( &thread1, NULL, blinking_led_function, (void*) line1);
    // iret2 = pthread_create( &thread2, NULL, blinking_led_function, (void*) line2);

    /*joing the thread before programs end*/
    while (i <= 20) {
		ret = gpiod_line_event_wait(line3, &ts);
		if (ret < 0) {
			perror("Wait event notification failed\n");
			ret = -1;
			goto release_line;
		} else if (ret == 0) {
			iret1 = pthread_create( &thread1, NULL, blinking_led_function, (void*) line1);
            iret2 = pthread_create( &thread2, NULL, blinking_led_function, (void*) line2);
			continue;
		}

		ret = gpiod_line_event_read(line3, &event);
		printf("Get event notification\n");
		if (ret < 0) {
			perror("Read last event notification failed\n");
			ret = -1;
			goto release_line;
		}
		sleep(1);

        i++;

        pthread_join( thread1, NULL);
        pthread_join( thread2, NULL);
    }

release_line:   //ending line 1 and line 2
    gpiod_line_release(line1);
    gpiod_line_release(line2);
    gpiod_line_release(line3);

close_chip:     //ending the chip
    gpiod_chip_close(chip);

ending:         //closing the main function
    return 0;
}

void *blinking_led_function( void *ptr ){
    int i, ret, val = 0;
    struct gpiod_line *line = ptr;

    for (i = 1; i > 0; i--) {
        ret = gpiod_line_set_value(line, val);
        sleep(1);
        val=!val;
    }
}
