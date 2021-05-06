'''from turtle import *
from contagiri import Contagiri
from geometry import Vector, Point

home()
speed("fast")

s = Point(0, 0)
c = Contagiri(s)

def add(x, y):
    n = Point(x, y)
    c.add(n)
    goto(x, y)

onscreenclick(add)
    
done()'''

from contagiri import Contagiri
from geometry import Vector, Point
import pygame
import sys

pygame.init()
pygame.font.init()
myfont = pygame.font.SysFont('Consolas', 30)

frames_per_second = 15
window_height = 1300
window_width = 1800

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
BLUE = (0, 0, 255)
RED = (255, 0, 0)
GREEN = (0, 255, 0)

display = pygame.display.set_mode((window_width, window_height))
clock = pygame.time.Clock()

c = None

file = open("track.txt", "w")

try:
    while True:
        clock.tick(frames_per_second)
        
        # event loop
        for event in pygame.event.get():
            if event.type == pygame.MOUSEMOTION and event.buttons[0] == True:
                
                point = Point(event.pos[0], event.pos[1])
                file.write(f"{point.x} {point.y}\n")
                if c is None:
                    c = Contagiri(point)
                else:
                    c.add(point)
                
                display.fill(BLACK)
                display.blit(myfont.render(str(c.c), False, WHITE),(100,100))
                pygame.draw.circle(display, RED, (c.p[0].x, c.p[0].y), 2, 2)
                for vec in c.v:
                    pygame.draw.line(display, BLUE, (vec.p1.x, vec.p1.y), (vec.p2.x, vec.p2.y))
                if c.line is not None:
                    pygame.draw.line(display, WHITE, (c.line.p1.x, c.line.p1.y), (c.line.p2.x, c.line.p2.y))
                pygame.draw.circle(display, RED, (c.p[0].x, c.p[0].y), c.range_th, 1)

            elif event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
        pygame.display.flip()
except KeyboardInterrupt as e:
    print("quitting")

file.close()