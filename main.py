# Importieren von Pygame Paketen
import pygame
import sys
import string

# Funktionen für das Grundgerüst damit Pygame laufen kann
pygame.init()
bildschirm = pygame.display.set_mode([1000, 550])
pygame.display.set_caption("Schiffe versenken")
hintergrund = pygame.image.load("Grafiken-SchiffeVersenken/hintergrund.jpg")

# Variablen
spielbrettbreite = 200
spielbretthoehe = 200
kästchengroesse = 40
blau = (0, 0, 255)
grün = (0, 255, 0)
rot = (255, 0, 0)
grau = (190, 190, 190)
schwarz = (0, 0, 0)
ABC_G = string.ascii_uppercase


def spielfeld_erstellen():
    bildschirm.blit(hintergrund, (0, 0))

    pygame.display.update()


go = True
while go:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()
    spielfeld_erstellen
