import json
from contagiri import Contagiri
from geometry import Vector, Point
import pygame

# PYGAME INIT
pygame.init()
pygame.font.init()
myfont = pygame.font.SysFont('Consolas', 30)
# PYGAME PARAMS
frames_per_second = 10
window_height = 1000
window_width = 1600
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
BLUE = (0, 0, 255)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
# PYGAME FINAL
display = pygame.display.set_mode((window_width, window_height))
clock = pygame.time.Clock()







c = None
x_multiplier = None
y_multiplier = None

try:
    with open("GPS_LOG_AND_PROCESS/1/result.json", "r") as f:
        data = json.load(f)["gps"]["rmc"]
    with open("GPS_LOG_AND_PROCESS/2/result.json", "r") as f:
        data += json.load(f)["gps"]["rmc"]
    
    for rmc in data:

        print(rmc)
        clock.tick(frames_per_second)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

        lat, lon = rmc["value"]["latitude"] - 46.4331, rmc["value"]["longitude"] - 11.3129
        point = Point(lat * 10**6, lon * 10**6)

        if c is None:
            c = Contagiri(point)
        else:
            c.add(point)
        
        display.fill(BLACK)
        display.blit(myfont.render(str(c.c), False, WHITE),(100,100))
        pygame.draw.circle(display, GREEN, (c.p[0].x, c.p[0].y), 6, 6)
        for vec in c.v:
            pygame.draw.line(display, BLUE, (vec.p1.x, vec.p1.y), (vec.p2.x, vec.p2.y), 2)
        if c.line is not None:
            pygame.draw.line(display, WHITE, (c.line.p1.x, c.line.p1.y), (c.line.p2.x, c.line.p2.y), 2)
        pygame.draw.circle(display, RED, (c.p[0].x, c.p[0].y), c.range_th, 1)

        pygame.display.flip()
    input("end")
except KeyboardInterrupt as e:
    pygame.quit()
    print("quitting")