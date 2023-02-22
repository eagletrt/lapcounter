#include <math.h>
#include <stdio.h>
#include <string.h>

#include <lapcounter/lapcounter.h>

#define MAX_CHAR 1000
#define MAX_POINTS 300000
#define MAX_LAPS 100

#define POSITION_THRESHOLD .000001

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <file>\n", argv[0]);
    return 1;
  }

  char row[MAX_CHAR];
  FILE *fp = fopen(argv[1], "r");
  if (!fp) {
    printf("Error opening file\n");
    return 1;
  }
  fgets(row, MAX_CHAR, fp); // skip header

  

  char *token;
  double timestamp;
  double last_timestamp = 0;
  int total_points = 0;
  lc_point_t point;

  double timestamps[MAX_POINTS];
  lc_point_t geo_points[MAX_POINTS];

  while (feof(fp) != true) {
    if (total_points == MAX_POINTS) {
      printf("Error: too many points\n");
      break;
    }

    fgets(row, MAX_CHAR, fp);

    token = strtok(row, ","); // timestamp
    
    timestamp = double(atoll(token))/1e6;

    last_timestamp = timestamp;

    token = strtok(NULL, ","); // longitude
    point.x = atof(token);
    point.y = atof(token + strlen(token)+1);

    if (point.x == 0 && point.y == 0)
      continue;

    timestamps[total_points] = timestamp;
    geo_points[total_points] = point;
    total_points++;
  }

  if (total_points == 0) {
    printf("Error: no points\n");
    return 1;
  }

  double lat = 0, lng = 0;

  int x_start = 0, x_end = 0, y_start = 0, y_end = 0;

  lc_counter_t *lc = lc_init(LC_DEFAULT_PROXIMITY_INCREMENT, LC_DEFAULT_INCLINATION_THRESHOLD,
                             LC_DEFAULT_DISTANCE_THRESHOLD, LC_DEFAULT_START_POINTS_COUNT);

  int increment_direction = 0;
  
  int test_lap_index = 0;
  const int iterations = 10;
  double lap_times[iterations][MAX_LAPS];
  for(int j = 0; j < iterations; j ++){
    lc_reset(lc);
    int prev_k = 0;
    test_lap_index = 0;
    for(int k = j; k < total_points; k++) {
      if(lc_eval_point(lc, &geo_points[k])) {
        lap_times[j][test_lap_index++] = timestamps[k] - timestamps[prev_k];
        prev_k = k;
      }
    }
  }
  printf("\r\n\n");
  double avg;
  double dev_std;
  double avg_avg = 0;
  double avg_std_dev = 0;
  for(int j = 0; j < iterations; j++){
    printf("Iter %02d    | ", j);
  }
  printf("\r\n");
  for(int k = 0; k < test_lap_index; k++){
    avg = 0;
    dev_std = 0;
    for(int j = 0; j < iterations; j ++) {
      printf("%010.6f | ", lap_times[j][k]);
      avg += lap_times[j][k];
    }
    avg /= iterations;
    for(int j = 0; j < iterations; j ++) {
      dev_std += pow(lap_times[j][k] - avg, 2);
    }
    dev_std /= iterations;
    dev_std = sqrt(dev_std);

    avg_avg += avg;
    avg_std_dev += dev_std;

    printf("AVG: %010.6f | DEV_STD: %010.6f\r\n", avg, dev_std);
  }
  avg_avg /= test_lap_index;
  avg_std_dev /= test_lap_index;
  printf("\r\n");
  printf("Average of the Averages: %f\r\n", avg_avg);
  printf("Average Standard Deviation: %f\r\n", avg_std_dev);
  printf("\r\n\n");

  return 0;
}
