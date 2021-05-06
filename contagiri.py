import math
from geometry import Vector, Point

class Contagiri:
    
    def __init__(self, start):
        self.p = [ start ] # Point object
        self.v = [] # Vector object
        self.angle_th = 45 #°
        self.range_th = 40 #px
        self.c = 0
        self.line = None
    
    def add(self, new):
        last = self.p[-1]
        vec = Vector(last, new)
        self.v.append(vec)
        self.p.append(new)

        # 1 - ACCAVALLAMENTO, above_startline: if last is before startline and new is after starline
        if self.line is None:
            self.line = self.v[0].perpendicular()
        eval_new = Vector(self.line.p1, new)
        eval_last = Vector(self.line.p1, last)
        res_new = Vector.determinant(self.line, eval_new) > 0
        res_last = Vector.determinant(self.line, eval_last) > 0
        above_startline = res_new == True and res_last == False

        # 2 - PROSSIMITà, in_range: if last and new are less than "threshold" away from start
        in_range = Point.distance(last, self.p[0]) < self.range_th and Point.distance(new, self.p[0]) < self.range_th

        # 3 - INCLINAZIONE, admissible_angle: if vec.angle is less than "threshold" away from start vector.angle
        new_angle = vec.angle()
        start_angle = self.v[0].angle()
        admissible_angle = new_angle < start_angle + self.angle_th and new_angle > start_angle - self.angle_th
        
        #print(f"range:{in_range}, angle:{admissible_angle}, above:{above_startline}")
        new_lap = above_startline and in_range and admissible_angle
        if new_lap:
            self.c += 1
            print(f"new lap ({self.c})")
        return new_lap

