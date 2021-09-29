# Eagle-Lapcounter

## Usage

### Inclusion
You must include the header file alongside all of your other libraries.
```
// MAIN.C
#include "../src/lapcounter.h"
```
In order to compile everything succesfully you must specify all the 3 .c files which make up the library.
```
// COMPILE.SH
gcc ../src/lapcounter.c \
    ../src/utils/vector/vector.c \
    ../src/utils/vector/point/point.c \
    main.c \
    -lm -o test.out
```

### Initialization
To make everything easier for everyone, the default setting are already dialed in and every specific threshold will be automatically adjusted. You just have to allocate the point (which will contain the GPS longitude and latitude) and init the structure.
```
lc_point_t point;   // to pass to evaluation
lc_counter_t * lp = lc_init(NULL);  // initialization with default settings
// you could use lc_config_t in order to override default
```

### Evaluation
There are two ways to know when the car passes the starting line. First of all you must evaluate the new point. Then you can catch the result of the evaluation to see if a new lap has started. To keep the exact count of the laps you can always access the property laps_count inside the lc_counter_t structure.
```
point.x = x; // longitude --> x, gathered from gps
point.y = y; // latitude --> y, gathered from gps
if (lc_eval_point(lp, &point)) { // return 1 if new lap
    // if new lap is found then print laps count
    printf("New lap! N%d\n", lp->laps_count);
}
```

### Reset
With the reset function you delete all the information about this particular session. Keep in mind that thresholds and configurations (such the ones of lc_config_t) will remain the same.
```
lc_reset(lp); // reset object (removes everything but thresholds)
```

### Destroy
By destroying the lc_counter_t you simply deallocate the memory that has been instanced. The only dynamic part of the object is the array of points used to determine the starting vector.
```
lc_destroy(lp);
```

## Principles
The lapcounter system bases its functionality on three simple properties, which help us compensate extreme cases and situations (like false positives). You can find more information on this [document](https://drive.google.com/file/d/1-MapkUXzzrGH4DVZvX76lGLyhz5Tq8yk/view?usp=sharing) (only in ITA).
- Proximity: new point and last point must be new the starting point (the distance is dinamically adjusted according to the longest vector);
- Inclination: current vector must have a similar inclination (angle and direction) to the starting vector;
- Overlap: current vector must overlap the start line (new point and last point must be on opposite sides of the start line).

## Get result
At the end of the day what matters most is how you can retrieve the laps count:
- `lc_eval_point(lp, &point)`: right after evaluation you can catch the return value to see if the current point has set a new lap;
- `lp->laps_count`: from LapCounter struct you can always retrieve the count of the laps.
