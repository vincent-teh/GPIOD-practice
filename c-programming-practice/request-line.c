#include <stdio.h>
#include <gpiod.h>
#include <unistd.h>

int main(){
    struct gpiod_chip* l_chip0 = gpiod_chip_open_by_number(0);

    struct gpiod_line* l_line0 = gpiod_chip_get_line(l_chip0, 0);

    struct gpiod_line_request_config l_sConfig = 
    {
        .consumer = "MyTest",
        .request_type = GPIOD_LINE_REQUEST_DIRECTION_OUTPUT
    };

    if (gpiod_line_request(l_line0, &l_sConfig, 0) != 0)
    {
        printf("request line fail\n");
        return -1;
    }

    sleep(10); //sleep for 10 seconds

    return 0;
}    
