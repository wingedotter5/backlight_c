#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/stat.h>

#define BL_PATH "/sys/class/backlight/amdgpu_bl0/brightness"


void print_usage() {
    fprintf(stderr, "Usage: backlight [OPTION] [VALUE]\nChange screen backlight brightness\n");
}


int inc_brightness(char* str_current, char* str_inc) {
    int current_val = strtol(str_current, NULL, 10);
    current_val += strtol(str_inc, NULL, 10);

    printf("Current brightness: %d\n", current_val);
    return current_val;
}


int dec_brightness(char* str_current, char* str_dec) {
    int current_val = strtol(str_current, NULL, 10);
    current_val -= strtol(str_dec, NULL, 10);

    return current_val;
}


int write_to_file(const char* str) {
    return 1;
}


int main(int argc, char *argv[]) {

    if (argc != 3) {
        print_usage();

    } else {
        // open file for changing screen brightness
        int fd = open(BL_PATH, O_RDWR);

        // If the brightness file is successfully opened
        if (fd > -1) {
            char fd_buff[64];
            char* buff_ptr = fd_buff;
            memset(fd_buff, 0, sizeof(fd_buff));

            // read in current brightness
            while (read(fd, buff_ptr, sizeof(char)) && *buff_ptr != '\n') {
                buff_ptr++;
            }
            *buff_ptr = '\0';

            // increase or decrease brightness
            if (!strcmp(argv[1], "inc")) {
                inc_brightness(fd_buff, argv[2]);

            } else if (!strcmp(argv[1], "dec")) {
                dec_brightness(fd_buff, argv[2]);

            } else {
                fprintf(stderr, "invalid option \'%s\'\n\n", argv[1]);
                print_usage();
            }

            // close the file after changing brightness
            if (close(fd) < 0)
                perror("error closing file");

            return 0;
        }
        perror("error changing backlight brightness");
    }

    return -1;
}
