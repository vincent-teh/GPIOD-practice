#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

int main(int argc, char const *argv[])
{
    //variables declaration
    char *chipname = "gpiochip0";       //gpio name; can be found by typing $gpioinfo
    //declaration of line to be used
	unsigned int line_num0 = 0;         //LED pin
    unsigned int line_num1 = 1;         //Push button pin

	struct timespec ts = { 10000, 0 };  //Waiting time out after 10000s
	struct gpiod_line_event event;
	struct gpiod_chip *chip;
    //declare line needed
	struct gpiod_line *line0;
    struct gpiod_line *line1;
    //other variables
	int i, ret;                         //"ret" represent return value of each gpiod process

    chip = gpiod_chip_open_by_name(chipname);
    if (!chip){
        printf("Open chip failed\n");
        ret = -1;
        goto end;
    }

    line0 = gpiod_chip_get_line(chip, line_num0);
    if (!line0){
        printf("Get line 0 failed\n");
        ret = -1;
        goto close_chip;
    }

    line1 = gpiod_chip_get_line(chip, line_num1);
    if (!line1){
        printf("Get line 1 failed\n");
        ret = -1;
        goto release_line_0;
    }

    ret = gpiod_line_request_output(line0, CONSUMER, 0);
    if (ret < 0){
        printf("Error request output for line 0\n");
        goto release_line_1;
    }

    ret = gpiod_line_request_both_edges_events(line1, CONSUMER);
    if (ret < 0) {
        printf("Failed request both edge triggered for line 1\n");
        goto release_line_1;
    }

    ret = gpiod_line_set_value(line0, 1);                 //initialise the led to off
        if (ret < 0) {
            printf("Error, failed to trigger led\n");
            goto release_line_1;
        }

    while (true)
    {
        ret = gpiod_line_event_wait(line1, &ts);
        if (ret < 0) {
            printf("Wait event notification failed\n");
            goto release_line_1;
        }else if (ret == 0)
        {
            printf("Wait event time out\n");
            goto release_line_1;
        }

        ret = gpiod_line_event_read(line1, &event);
        if (ret < 0) {
            printf("Error read last event notification failed\n");
            goto release_line_1;
        }

        switch (event.event_type)
        {
        case GPIOD_LINE_EVENT_RISING_EDGE:{
            ret = gpiod_line_set_value(line0, 1);
            if (ret < 0) {
                printf("Error, failed to turn on led\n");
                goto release_line_1;
                }
            break;
            }

        case GPIOD_LINE_EVENT_FALLING_EDGE:{
            ret = gpiod_line_set_value(line0, 0);
            if (ret < 0) {
            printf("Error, failed to turn off led\n");
            goto release_line_1;
            }
            break;
            }

        default:{
            break;
            }
        }
    }

release_line_1:
    gpiod_line_release(line1);

release_line_0:
    gpiod_line_release(line0);

close_chip:
    gpiod_chip_close(chip);

end:
    return ret;
}
