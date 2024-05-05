#include "controller.h"
/*enum status_bits{
    INIT=1,
    RECORD_START=2,
    RECORD_STOP=4,
    SNAPSHOT=8,
    CAPTURE=16,
    RGB_CONVERTER=32,
    YUV_CONVERTER=64,
    ENCODE=128,
    WRITE_RECORD=256
};*/

int flash_open(){
#ifdef ARM
    int fd = open("/sys/class/gpio/export", O_WRONLY);
        if (fd == -1) {
            perror("Unable to open /sys/class/gpio/export");
            exit(1);
        }

        if (write(fd, "24", 2) != 2) {
            perror("Error writing to /sys/class/gpio/export");
            exit(1);
        }

        close(fd);

        // Set the pin to be an output by writing "out" to /sys/class/gpio/gpio24/direction

        fd = open("/sys/class/gpio/gpio24/direction", O_WRONLY);
        if (fd == -1) {
            perror("Unable to open /sys/class/gpio/gpio24/direction");
            exit(1);
        }

        if (write(fd, "out", 3) != 3) {
            perror("Error writing to /sys/class/gpio/gpio24/direction");
            exit(1);
        }

        close(fd);

        fd = open("/sys/class/gpio/gpio24/value", O_WRONLY);
        if (fd == -1) {
            perror("Unable to open /sys/class/gpio/gpio24/value");
            exit(1);
        }
        return fd;
#endif
}

void flash_close(int fd){
    #ifdef ARM
    close(fd);

     // Unexport the pin by writing to /sys/class/gpio/unexport

     fd = open("/sys/class/gpio/unexport", O_WRONLY);
     if (fd == -1) {
         perror("Unable to open /sys/class/gpio/unexport");
         exit(1);
     }

     if (write(fd, "24", 2) != 2) {
         perror("Error writing to /sys/class/gpio/unexport");
         exit(1);
     }

     close(fd);
#endif
}
void stop_flash_it(int fd){
 //   printf("stop flashing\n");
#ifdef ARM
    if (write(fd, "0", 1) != 1) {
                perror("Error writing to /sys/class/gpio/gpio24/value");
                exit(1);
            }
#endif
    }
void flash_it(int fd){
 //   printf("flashing\n");
#ifdef ARM
    if (write(fd, "1", 1) != 1) {
                perror("Error writing to /sys/class/gpio/gpio24/value");
                exit(1);
            }
#endif

}

void* flash_fun(void*arg){
    struct flash  *my_struct=((struct flash*)arg);
    enum STATUS_MACHINE cur_status;

    switch (my_struct->time) {
    case RECORD_LONG:cur_status=BLINK_ON;
                break;
    case SNAPSHOT_LONG:cur_status=SNAPSHOT_ON;
        break;
    }
    int fd=flash_open();



    while(my_struct->is_active){
        switch (cur_status) {
        case BLINK_ON:
            flash_it(fd);
            cur_status=BLINK_OFF;
            usleep(my_struct->time);
            break;
         case BLINK_OFF: stop_flash_it(fd);
            cur_status=BLINK_ON;
            usleep(my_struct->time);
            break;
        case SNAPSHOT_ON:flash_it(fd);
            printf("snapshot------\n");
            cur_status=SNAPSHOT_OFF;
            usleep(my_struct->time);
            break;
        case SNAPSHOT_OFF:
            stop_flash_it(fd);
           my_struct->is_active=0;
           break;
        }
    }

    flash_close(fd);
}

void* contoller_fun(void* arg){

    int* status=(int*)arg;
    int prev=0;
    pthread_t flash,blink;
    struct flash record_struct, snapshot_struct;
    while(*status){
        printf("--controller status:%d--\n",*status);
        if(((*status) & RECORD_START)&&!((prev) & RECORD_START)){
            record_struct.time=RECORD_LONG;
            record_struct.is_active=1;
            pthread_create(&blink,NULL,flash_fun,&record_struct);
        }
        if(((*status) & RECORD_STOP)&&!((prev) & RECORD_STOP)){
            record_struct.is_active=0;
            pthread_join(blink,NULL);
        }
        if(((*status) & SNAPSHOT)&&!((prev) & SNAPSHOT)){
            snapshot_struct.time=SNAPSHOT_LONG;
            pthread_create(&flash,NULL,flash_fun,&snapshot_struct);
        }

            //checks status and act
        prev=*status;
        sleep(2);
        }

    }


