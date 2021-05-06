# eagle-lapcounter

## Usage
- CREATE LapCounter
- EVALUATE Point
- RESET LapCounter
- DESTROY LapCounter

## Principles
- Proximity: new point and last point must be new the starting point (the distance is dinamically adjusted according to the longest vector);
- Inclination: current vector must have a similar inclination (angle and direction) to the starting vector;
- Overlap: current vector must overlap the start line (new point and last point must be on opposite sides of the start line).

## Get result
Two methods:
- from EVALUATION you can see if the current point has set a new lap (True/False)
- from LapCounter struct you can retrieve the count and the list of the laps
