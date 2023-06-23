#include <math.h>
#include <stdio.h>
#include <string.h>

#include <lapcounter/lapcounter.h>
#include <raylib.h>

#define MAX_CHAR 1000
#define MAX_POINTS 200000
#define MAX_LAPS 100

#define POSITION_THRESHOLD .000001

#define A_SIN(x) ((sin(x) + 1.)) / 2.
#define A_COS(x) ((cos(x) + 1.)) / 2.

#if defined(WIN32)
#define SEP "\\"
#else
#define SEP "/"
#endif

int mod(int a, int b) {
  int r = a % b;
  return r < 0 ? r + b : r;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <file>\n", argv[0]);
    return 1;
  }

  FILE *fp = fopen(argv[1], "r");

  if (!fp) {
    printf("Error opening file\n");
    return 1;
  }

  char row[MAX_CHAR];
  char *token;

  double timestamp;
  lc_point_t point;

  int total_points = 0;

  double timestamps[MAX_POINTS];
  lc_point_t points[MAX_POINTS];
  lc_point_t geo_points[MAX_POINTS];

  int sectors_index[] = {76, 137};

  int total_laps = 0;
  int laps[MAX_LAPS];

  fgets(row, MAX_CHAR, fp); // skip header

  float xmax = -INFINITY, xmin = INFINITY, ymax = -INFINITY, ymin = INFINITY;

  while (feof(fp) != true) {
    if (total_points == MAX_POINTS) {
      printf("Error: too many points\n");
      break;
    }

    fgets(row, MAX_CHAR, fp);

    token = strtok(row, ","); // timestamp
    timestamp = double(atoll(token))/1e6;

    token = strtok(NULL, ",");
    point.x = atof(token); // longitude
    point.y = atof(token + strlen(token)+1); // latitude

    if (point.x == 0 && point.y == 0)
      continue;

    if (point.x > xmax)
      xmax = point.x;
    if (point.x < xmin)
      xmin = point.x;
    if (point.y > ymax)
      ymax = point.y;
    if (point.y < ymin)
      ymin = point.y;

    timestamps[total_points] = timestamp;
    points[total_points] = point;
    geo_points[total_points] = point;
    total_points++;
  }
  if (total_points == 0) {
    printf("Error: no points\n");
    return 1;
  }


  float range = std::fabs(std::max(xmax - xmin, ymax - ymin));
  xmax = xmax + range * .2;
  xmin = xmin - range * .2;
  ymax = ymax + range * .2;
  ymin = ymin - range * .2;

  // Drawing stuff

  const int width = 1400;
  const int height = 1000;

  double xscale = 1.1, yscale = 1.1, xoffset = 0, yoffset = 0;
  double scale = fmin(((float)width) / (xmax - xmin), ((float)height) / (ymax - ymin));

  yoffset = (height - (ymax - ymin) * scale) / 2.0;
  xoffset = (width - (xmax - xmin) * scale) / 2.0;

#define XX(x) (xoffset + ((x)-xmin) * scale)
#define YY(y) (height - (yoffset + ((y)-ymin) * scale))

  for (int j = 0; j < total_points; j++) {
    points[j].x = XX(points[j].x);
    points[j].y = YY(points[j].y);
  }

  SetTraceLogLevel(LOG_WARNING | LOG_ERROR);
  InitWindow(width, height, "\U0001D4DB\U0001D4EE lapcounter");
  SetTargetFPS(60);

  int point_index = 0, lap_index = 0, lap_iterator = 0;
  bool autoplay = false;

  double lat = 0, lng = 0;
  char buffer[MAX_CHAR];

  int speed = 1;
  double lap_time = 0;
  double last_lap_time = 0;
  double best_lap_time = INFINITY;

  unsigned char r = 0, g = 0, b = 0, a = 0;
  int j = 0;

  int x_start = 0, x_end = 0, y_start = 0, y_end = 0;
  int sector = 0;

  lc_counter_t *lc = lc_init(LC_DEFAULT_PROXIMITY_THRESHOLD, LC_DEFAULT_INCLINATION_THRESHOLD,
                             LC_DEFAULT_DISTANCE_THRESHOLD, LC_DEFAULT_START_POINTS_COUNT, sizeof(sectors_index)/sizeof(int)+1);
                             
  // lc_eval_point(lc, &geo_points[point_index]);

  int increment_direction = 0;

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_SPACE)) {
      autoplay = !autoplay;
    }

    if (IsKeyPressed(KEY_R)) {
      autoplay = false;
      point_index = 0;
      lc_reset(lc);
    }

    if (IsKeyPressed(KEY_S)) {
      lc_reset(lc);
    }

    if(IsKeyPressed(KEY_L)) {   //save
      lc_save(lc, "save.bin");
    }
    if(IsKeyPressed(KEY_K)) {
      lc_load(lc, "save.bin");
      printf("loaded %d sectors\n", lc->sector_count);
      printf("%f\n", lc->points[0].x);
    }

    if (IsKeyPressed(KEY_UP)) {
      speed = speed == 1 ? 2 : fmin(speed + 2, 50);
    } else if (IsKeyPressed(KEY_DOWN)) {
      speed = fmax(speed - 2, 1);
    }

    if (autoplay || IsKeyPressed(KEY_RIGHT) || (IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT_SHIFT))) {
      increment_direction = 1;
    } else if (IsKeyPressed(KEY_LEFT) || (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_LEFT_SHIFT))) {
      increment_direction = -1;
    }else {
      increment_direction = 0;
    }

    if(increment_direction != 0){
      for(int i = 0; i < speed; i++) {
#ifdef ADD_SECTORS
        if(point_index == sectors_index[sector])
          lc_add_sector(lc, &geo_points[point_index]);
#endif
        int tmp = lc_eval_point(lc, &geo_points[point_index]);
        if (tmp == 0) {
          laps[lap_index++] = point_index;
        }
        if(tmp >= 0)
          sector = tmp;
        point_index = mod(point_index + increment_direction, total_points);
      }
    }

    lng = (GetMouseX() / scale) + xmin;
    lat = (GetMouseY() / scale) + ymin;

    lap_time = timestamps[point_index] - timestamps[(lap_index - 1 >= 0) ? laps[lap_index - 1] : 0];

    if (lap_index > 0) {
      last_lap_time = timestamps[laps[lap_index - 1]] - timestamps[(lap_index - 2 >= 0) ? laps[lap_index - 2] : 0];
      for (j = 0; j < lap_index; j++) {
        best_lap_time = fmin(best_lap_time, timestamps[laps[j]] - timestamps[(j - 1 >= 0) ? laps[j - 1] : 0]);
      }
    } else {
      last_lap_time = 0;
      best_lap_time = INFINITY;
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);

    lap_iterator = 0;
    for (j = 1; j < point_index; j++) {
      r = static_cast<unsigned char>(255 * A_SIN(lap_iterator));
      g = static_cast<unsigned char>(255 * A_COS(lap_iterator));
      b = static_cast<unsigned char>(255 * A_COS(lap_iterator + M_PI / 2));

      a = lap_iterator == lap_index ? 255 : 50;
      Vector2 p1 = {points[j - 1].x, points[j - 1].y};
      Vector2 p2 = {points[j].x, points[j].y};
      DrawLineEx(p1, p2, 5, (Color){r, g, b, a});

      if (laps[lap_iterator] == j) {
        lap_iterator++;
      }
    }

    DrawLine(XX(lc->start_points[0].x), YY(lc->start_points[0].y),
             XX(lc->start_points[0].x + lc->start_inclination_vector.x * 10),
             YY(lc->start_points[0].y + lc->start_inclination_vector.y * 10), RED);

    DrawLine(XX(lc->start_points[0].x), YY(lc->start_points[0].y),
             XX(lc->start_points[0].x + lc->start_line_vector.x * 10),
             YY(lc->start_points[0].y + lc->start_line_vector.y * 10), BLUE);

    DrawLine(XX(lc->current_point.x), YY(lc->current_point.y), XX(lc->current_point.x + lc->current_vector.x * 10),
             YY(lc->current_point.y + lc->current_vector.y * 10), VIOLET);

    for(int i=0;i<sizeof(sectors_index)/sizeof(int);i++) {
      double x = geo_points[sectors_index[i]].x;
      double y = geo_points[sectors_index[i]].y;
      DrawCircle(XX(x), YY(y), 10, RED);
    }

    sprintf(buffer, "Lap time: %.3f\nBest: %.3f\nLast: %.3f", lap_time, best_lap_time, last_lap_time);
    DrawText(buffer, 10, 10, 20, BLACK);

    sprintf(buffer, "Current sector: %d\n", sector);
    DrawText(buffer, 10, 100, 20, BLACK);

    sprintf(buffer, "Lap: %d\nPoint: %d\nSpeed: %dx\nInclination: %d\nProximity %d\nOverlap: %d", lap_index + 1,
            point_index + 1, speed, lc->last_inclination_result, lc->last_proximity_result, lc->last_overlap_result);
    DrawText(buffer, 10, height - 500, 20, BLACK);

    sprintf(buffer, "%f\n%f", lat, lng);
    DrawText(buffer, 10, height - 300, 20, BLACK);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      SetClipboardText(buffer);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}