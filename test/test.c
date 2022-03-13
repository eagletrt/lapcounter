#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "../src/lapcounter.h"

typedef struct __is_packed {
    float latitude;
    float longitude;
} Primary_GPS_COORDS;

size_t deserialize_Primary_GPS_COORDS(uint8_t* buffer, Primary_GPS_COORDS* primary_gps_coords) {
    memcpy(primary_gps_coords, buffer, sizeof(Primary_GPS_COORDS));
    return sizeof(Primary_GPS_COORDS);
}


void main(int argc, char **argv) {

    double time_spent = 0.0;
    int bytes_read;     // to remove warning on fscanf

    int s, j;
    int nBytesRead;
    struct sockaddr_can addr1;
    struct sockaddr_can addr2;
    struct can_frame frame;
    struct ifreq ifr1;
    struct ifreq ifr2;

    const char *ifname1 = "vcan0";
    const char *ifname2 = "can0";

    
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) == -1) {
        perror("Error while opening socket for reading virtual can");
        return;
    }
    /*
    if ((j = socket(PF_CAN, SOCK_RAW, CAN_RAW)) == -1) {
        perror("Error while opening socket for writing can");
        return;
    }*/

    strcpy(ifr1.ifr_name, ifname1);
    //strcpy(ifr2.ifr_name, ifname2);
    ioctl(s, SIOCGIFINDEX, &ifr1);
    //ioctl(j, SIOCGIFINDEX, &ifr2);

    addr1.can_family  = AF_CAN;
    addr1.can_ifindex = ifr1.ifr_ifindex;
    //addr2.can_family  = AF_CAN;
    //addr2.can_ifindex = ifr2.ifr_ifindex;

    printf("%s at index %d\n", ifname1, ifr1.ifr_ifindex);
    //printf("%s at index %d\n", ifname2, ifr2.ifr_ifindex);

    if (bind(s, (struct sockaddr *)&addr1, sizeof(addr1)) == -1) {
            perror("Error in socket bind");
            return;
    }
    // Writing on can that lapcounter started
    frame.can_id  = 0x6A0;
    frame.can_dlc = 4;
    frame.data[0] = 0x12;
    frame.data[1] = 0x34;
    frame.data[2] = 0x56;
    frame.data[3] = 0x78;


    nBytesRead = write(s, &frame, sizeof(struct can_frame));  //write on canbus that lapcounter started
    if (nBytesRead == -1) {
        fprintf(stderr, "Tried to write on %s but failed. Is it on?\n", ifname1);
        return;
    }

    lc_point_t point;   // to pass to evaluation
    lc_counter_t * lp = lc_init(NULL);  // initialization with default settings
    lc_counter_t * lp_inclination = lc_init(NULL);
    double precX = -1;
    double precY = -1;

    while (scanf("%lf %lf", &point.x, &point.y)) {
        if (point.x == precX && point.y == precY)
            continue;
        precX = point.x;
        precY = point.y;
        printf("Reading: %lf, %lf\n", point.x, point.y);
        if (point.x == 0 && point.y == 0)
            break;
        if (lc_eval_point(lp, &point, lp_inclination)) {
            struct can_frame frameWrite;
            frameWrite.can_id  = 0x6A0;  //change this
            frameWrite.can_dlc = 1; // 1 for lap count
            uint8_t lapCount = lp->laps_count;  //check if this overwrites data on frame
            frameWrite.data[0] = lapCount;
            int nbytes = write(s, &frameWrite, sizeof(struct can_frame));
            printf("BIP!\n");
        }
    }
    printf("\nLAP COUNT: %d\n", lp->laps_count);

    lc_reset(lp); // reset object (removes everything but thresholds)
    lc_destroy(lp);
    lc_reset(lp_inclination);
    lc_destroy(lp_inclination);
}
