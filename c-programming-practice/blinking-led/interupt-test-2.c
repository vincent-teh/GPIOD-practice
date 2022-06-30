#include <stdio.h>
#include <gpiod.h>
#include <pthread.h>
#include <unistd.h>

void *blinking_led_function( void *ptr );
void *detecting_interrupt( void *ptr);

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

int FLAG = 0;

int main(int argc, char const *argv[])
{
    pthread_t thread1, thread2;
    char *chipname = "gpiochip0";
	unsigned int line_num0 = 0;     //Blue led line
    unsigned int line_num1 = 1;     //Push button line
	struct gpiod_line_event event;
	struct gpiod_chip *chip;
	struct gpiod_line *line0;
    struct gpiod_line *line1;
    struct gpiod_line *line2;
    int i, ret;

    //Initialisation process
    chip = gpiod_chip_open_by_name(chipname);
    line0 = gpiod_chip_get_line(chip, line_num0);
    line1 = gpiod_chip_get_line(chip, line_num1);
    ret = gpiod_line_request_output(line0, CONSUMER, 0);
    ret = gpiod_line_request_rising_edge_events(line1, CONSUMER);    //ask the gpiod to stop at rising edge

    ret = pthread_create( &thread1, NULL, blinking_led_function, (void*) line0);
    ret = pthread_create( &thread2, NULL, detecting_interrupt, (void*) line1);

    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);

release_line:   //ending line 1 and line 2
    gpiod_line_release(line0);
    gpiod_line_release(line1);

close_chip:     //ending the chip
    gpiod_chip_close(chip);

ending:         //closing the main function
    return 0;
}

void *blinking_led_function( void *ptr ){
    int i, ret, val = 0;
    struct gpiod_line *line = ptr;

    while (true)
    {
        ret = gpiod_line_set_value(line, FLAG);
    }
}

void *detecting_interrupt( void *ptr){
    int ret;
    struct gpiod_line *line = ptr;
    struct gpiod_line_event event;
    struct timespec ts = { 10000, 0 };

    while(true){
        ret = gpiod_line_event_wait(line, &ts);
        ret = gpiod_line_event_read(line, &event);
        FLAG = !FLAG;
    }
}
