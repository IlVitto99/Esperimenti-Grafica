#!/usr/bin/env python3

from pygame import *
from random import randint
from sys import argv
import numpy as np
from random import random, seed
seed(65465+46854654)

init()

h = 700
w = 1200

screen = display.set_mode((w,h))
solot = time.Clock()

pa = Surface((40,40), SRCALPHA)
draw.circle(pa, (0,0,0), (20, 20), 20)

po = Surface((40,40), SRCALPHA)
draw.circle(po, (255,0,0), (20, 20), 5)

fine = False

distanza = lambda x1, y1, x2, y2 : (x1 - x2) ** 2 + (y1 - y2) ** 2

def getMode(x, y):
    return (x ** 2 + y ** 2) ** 0.5

def setMod(x, y, m):
    vm = getMode(x, y)
    return (x * (1 / vm) * m, y * (1 / vm) * m)

class layer():
    def __init__(self, nValore, nValorePre):
        self.valori = np.array([0 for _ in range(nValore)])
        if nValorePre != 0:
            self.bies = np.array([random() for _ in range(nValore)])
            self.pesi = np.array([random() for _ in range(nValore * nValorePre)]).reshape(nValorePre, nValore) 
            
class nn():
    def __init__(self,lista):
        self.layers = []
        self.layers.append(layer(lista[0], 0))
        for k,i in enumerate(lista):
            if k != 0:
                self.layers.append(layer(lista[k], lista[k-1]))
                
    def feedforward(self,inputs):
        if inputs.size == self.layers[0].valori.size:
            self.layers[0].valori = inputs
            for k,i in enumerate(self.layers):       
                if k != 0:
                    self.layers[k].valori = self.layers[k-1].valori.dot(self.layers[k].pesi) + self.layers[k].bies 
        
            return self.layers[-1].valori

class ogetto():
    tipo = "a"
    raggio = 0
    sprite = pa

    def __init__(self, x, y, dx, dy):
        self.x = x
        self.y = y
        self.dx = dx
        self.dy = dy
        self.vita = 1
        self.time = 0


    def atack(self):
        if self.time == 0:
            m = (self.dx ** 2 + self.dy ** 2) ** 0.5
            nx = self.dx * (1 / m) * 40 + self.x 
            ny = self.dy * (1 / m) * 40 + self.y 
            ndx = self.dx * (1 / m) * 40
            ndy = self.dy * (1 / m) * 40
            self.time = 200

            return proietile(nx, ny, ndx, ndy)


    def collisione(self, linstaOgetti):
        for o in linstaOgetti:
            if o != self:
                d = distanza(self.x, self.y, o.x, o.y) 
                if d < (self.raggio ** 2 + o.raggio ** 2) and d != 0:

                    if self.tipo == "p" and o.tipo == "g":
                        o.vita -= 1
                        return self
         
                    mv = getMode(self.dx, self.dy)
                    ndx, ndy = setMod(o.x - self.x, o.y - self.y, mv)
                    self.dx = -ndx
                    self.dy = -ndy

                
    def RunOneFrame(self):
        if self.time > 0:
            self.time -= 1
            if self.tipo == "p" and self.time <= 0:
                return self

        if self.vita <= 0:
            return self

        self.x += self.dx
        self.y += self.dy

        if (self.x >= w-self.raggio*2 or self.x <= 0):
            self.dx = -self.dx
            self.x += self.dx
        if (self.y >= h-self.raggio*2 or self.y <= 0):
            self.dy = -self.dy
            self.y += self.dy

    def setxy(self, x, y):
        self.x = x
        self.y = y

    def setv(self,dx, dy):
        self.dx = dx
        self.dy = dy

    def drow(self):
        screen.blit(self.sprite, (self.x, self.y))

    @staticmethod
    def run(lista, Edorw, time):
        while time > 0:
            for o in lista:
                if o.tipo == "g":
                    ioi = 0
                    for i in lista:
                        if i != o:
                            ioi = i
                    ris = o.ai(ioi)
                    if ris != None:
                        lista.append(ris)

            for o in lista:
                ris = o.collisione(lista)
                if ris != None:
                    lista.remove(ris)

            for o in lista:
                ris = o.RunOneFrame()
                if ris != None:
                    lista.remove(ris)

            if Edorw:
                screen.fill((255, 255, 255))
                for o in lista:
                    o.drow()
                solot.tick(30)
          
            time -= 1

        return lista


class gicatore(ogetto):
    tipo = "g"
    raggio = 20
    sprite = pa
    index = 0

    def __init__(self, x, y, dx, dy):
        ogetto.__init__(self, x, y, dx, dy)
        self.cervello = nn([12,12,10,3])
        self.index = gicatore.index
        gicatore.index += 1

    def ai(self, enemy):
        ris = self.cervello.feedforward(np.array([self.vita, self.time,self.x,self.y,self.dx,self.dy, enemy.x,enemy.y,enemy.dx,enemy.dy, enemy.vita, enemy.time]))
        nx, ny = setMod(ris[0], ris[1],20)
        self.setv(nx, ny)
        if ris[2] > 0: 
            return self.atack()



class proietile(ogetto):
    tipo = "p"
    raggio = 5
    sprite = po

    def __init__(self, x, y, dx, dy):
        ogetto.__init__(self, x, y, dx, dy)
        self.time = 60
        

li = [gicatore(200,200, 5, 5), gicatore(200, 400, -5, -10)]

while not fine:
    for e in event.get():
        if e.type == QUIT:
            fine = True

    li = ogetto.run(li, True, 1)
    display.flip()