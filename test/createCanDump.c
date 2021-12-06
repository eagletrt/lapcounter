#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <unistd.h>

#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <errno.h>


#include "../src/lapcounter.h"

typedef struct __is_packed {
    float latitude;
    float longitude;
} Primary_GPS_COORDS;

size_t serialize_Primary_GPS_COORDS(uint8_t* buffer, float latitude, float longitude) {
    Primary_GPS_COORDS primary_gps_coords = { latitude, longitude };
  // assert(buf_len >= sizeof(Primary_GPS_COORDS));
    memcpy(buffer, &primary_gps_coords, sizeof(Primary_GPS_COORDS));
    return sizeof(Primary_GPS_COORDS);
}

int main(int argc, char **argv) {

    int bytes_read;     // to remove warning on fscanf
    int s;
    int nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;

    const char *ifname = "vcan0";

    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) == -1) {
            perror("Error while opening socket");
            return -1;
    }

    strcpy(ifr.ifr_name, ifname);
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    printf("%s at index %d\n", ifname, ifr.ifr_ifindex);

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
            perror("Error in socket bind");
            return -1;
    }


    FILE *file = fopen("mattiaCoords.txt", "r");
    while (!feof(file)) {
        float lat, lon;
        bytes_read = fscanf(file, "%f %f\n", &lat, &lon);
        printf("%f, %f", lat, lon);
        uint8_t* buffer_primary_gps_coords = (uint8_t*)malloc(sizeof(Primary_GPS_COORDS));
        Primary_GPS_COORDS primary_gps_coords_s = { lat, lon };
        serialize_Primary_GPS_COORDS(buffer_primary_gps_coords, primary_gps_coords_s.latitude, primary_gps_coords_s.longitude);

        struct can_frame frame;
        frame.can_id=0x6A0;
        frame.can_dlc=8;
        frame.data[0]=buffer_primary_gps_coords[0];
        frame.data[1]=buffer_primary_gps_coords[1];
        frame.data[2]=buffer_primary_gps_coords[2];
        frame.data[3]=buffer_primary_gps_coords[3];
        frame.data[4]=buffer_primary_gps_coords[4];
        frame.data[5]=buffer_primary_gps_coords[5];
        frame.data[6]=buffer_primary_gps_coords[6];
        frame.data[7]=buffer_primary_gps_coords[7];

        int bytes;
        if ((bytes=write(s, &frame, sizeof(struct can_frame))) != sizeof(struct can_frame)) {
            perror("Write");
            exit(32);
        }


        //Primary_GPS_COORDS* primary_gps_coords_d = (Primary_GPS_COORDS*)malloc(sizeof(Primary_GPS_COORDS));
    }
}
