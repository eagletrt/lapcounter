import math

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def distance(p1, p2):
        v = Vector(p1, p2)
        return v.length()
    def __str__(self):
        return f"Point: {self.x};{self.y}"

class Vector:

    def __init__(self, p1, p2):
        self.p1 = p1
        self.p2 = p2
        self.a = p2.x - p1.x
        self.b = p2.y - p1.y
            
    def length(self):
        return math.sqrt(self.a**2+self.b**2)
    def angle(self):
        axis = Vector.getAxis()
        det = Vector.determinant(self, axis)
        dot = Vector.dotproduct(self, axis)
        radians = math.atan2(det, dot)
        return radians * 180 / math.pi
    def perpendicular(self):
        multiplier = 1 / self.length() * 40
        x1, y1 = self.p1.x + self.b * multiplier, self.p1.y- self.a * multiplier
        p1 = Point(x1, y1)
        x2, y2 = self.p1.x - self.b * multiplier, self.p1.y + self.a * multiplier
        p2 = Point(x2, y2)
        return Vector(p1, p2)

    def determinant(v1, v2):
        return - (v1.a * v2.b - v1.b * v2.a)
    def dotproduct(v1, v2):
        return v2.a * v1.a + v2.b * v1.b
    def getAxis():
        p1 = Point(0, 0)
        p2 = Point(1, 0)
        return Vector(p1, p2)

