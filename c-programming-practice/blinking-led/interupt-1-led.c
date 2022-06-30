#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

int main(int argc, char **argv)
{
	char *chipname = "gpiochip0";
	unsigned int line_num1 = 1;	// GPIO Pin #25
	unsigned int line_num2 = 2;
	struct timespec ts = { 1000, 0 };
	struct gpiod_line_event event;
	struct gpiod_chip *chip;
	struct gpiod_line *line1;
	struct gpiod_line *line2;
	int i, ret, ret_led;
	int val = 0;

	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		ret = -1;
		goto end;
	}

	line1 = gpiod_chip_get_line(chip, line_num1);
	if (!line1) {
		perror("Get line failed\n");
		ret = -1;
		goto close_chip;
	}

	line2 = gpiod_chip_get_line(chip, line_num2);
	gpiod_line_request_output(line2, CONSUMER, 0);
	gpiod_line_request_rising_edge_events(line1, CONSUMER);

	/* Notify event up to 20 times */
	i = 0;
	while (true) {
		ret = gpiod_line_event_wait(line1, &ts);
		printf("%d \t", ret);
		if (ret < 0) {
			perror("Wait event notification failed\n");
			ret = -1;
			goto release_line;
		} else if (ret == 0) {
			printf("Wait event notification on line #%u timeout\n", line_num1);
			gpiod_line_set_value(line2, val);
			val=!val;
			continue;
		}

		ret = gpiod_line_event_read(line1, &event);
		printf("Get event notification on line #%u %d times\n", line_num1, ret);
		if (ret < 0) {
			perror("Read last event notification failed\n");
			ret = -1;
			goto release_line;
		}

		i++;
	}

	ret = 0;

release_line:
	gpiod_line_release(line1);
	gpiod_line_release(line2);
close_chip:
	gpiod_chip_close(chip);
end:
	return ret;
}