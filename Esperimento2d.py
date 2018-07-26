#!/usr/bin/env python3

from pygame import *
from random import randint
from sys import argv
init()

h = 920
w = 1900

screen = display.set_mode((w,h))
solot = time.Clock()


fine = False

def distanza(a, b, c, d):
    return (a-c) ** 2 + (b - d) ** 2

class pallina():
    l = []
    index = 0
    def __init__(self, x, y, dx, dy, dc):
        self.x = x
        self.y = y
        self.dx = dx
        self.dy = dy
        self.col = [255,0,0]
        self.i = 0
        self.dc = dc
        self.index = pallina.index
        pallina.index += 1 
        self.sprite =  Surface((40,40),SRCALPHA)
        draw.circle(self.sprite,self.col,(20,20),20)
        pallina.l.append(self)

    def coll(self):
        for i in pallina.l[:self.index-1] + pallina.l[self.index:]:
            dis = distanza(self.x + 20, self.y +20, i.x+20, i.y+20)
            if dis < 1600 and dis != 0:
                mv = (self.dx ** 2  + self.dy ** 2) ** 0.5
                md = dis ** 0.5
                self.dx = -(i.x - self.x) * (1/md) * mv
                self.dy = -(i.y - self.y) * (1/md) * mv

    def draw(self):
        self.x += self.dx
        self.y += self.dy

        n = (self.i+1)%3
        self.col[self.i] -= self.dc
        self.col[n] += self.dc
        
        if self.col[self.i] < 0 or self.col[n] > 255:
            self.col[self.i] = 0
            self.col[n] = 255
            self.i += 1
            self.i = self.i % 3


        draw.circle(self.sprite,self.col,(20,20),20)
        if (self.x >= w-40 or self.x <= 0):
            self.dx = -self.dx
        if (self.y >= h-40 or self.y <= 0):
            self.dy = -self.dy
        screen.blit(self.sprite, (self.x, self.y))


numeropalline = int(argv[1])

for _ in range(numeropalline):
    pallina(randint(200, 400), randint(200, 400), randint(-20, 20), randint(-20, 20), randint(0, 50))

ii = 0
c = [255, 0, 0]
while not fine:
    for e in event.get():
        if e.type == QUIT:
            fine = True

    n = (ii + 1) % 3
    c[ii] -= 1
    c[n] += 1


    if c[ii] < 0 or c[n] > 255:
        c[ii] = 0
        c[n] = 255
        ii += 1
        ii = ii % 3

    screen.fill(c)

    for i in pallina.l:
        i.coll()

    for i in pallina.l:
        i.draw()

    display.flip()

    solot.tick(60)


quit()