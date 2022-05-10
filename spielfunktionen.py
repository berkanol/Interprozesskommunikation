'''Importieren'''
import pygame
import sys

'''Pygame zum Starten bringen'''
pygame.init()
bildschirm = pygame.display.set_mode([1000, 550])
pygame.display.set_caption("Schiffe versenken")
hintergrund = pygame.image.load("Grafiken-SchiffeVersenken/hintergrund.jpg")

'''Variablen werden initialisiert'''
blau = (0, 0, 255)
grun = (0, 255, 0)
rot = (255, 0, 0)
grau = (190, 190, 190)
schwarz = (0, 0, 0)
kaestchengroesse = 40
felderhorizontal = 10
feldervertikal = 10


'''Grid (Gitter) wird erstellt, um die Basis des Spielfeldes zu erschaffen'''
def spielfeld_erstellen():

    bildschirm.blit(hintergrund, (0, 0))
    for x in range(0, (felderhorizontal + 1) * kaestchengroesse, kaestchengroesse):
        pygame.draw.line(bildschirm, schwarz, (x + 60, 120), (x + 60, 520))
    for y in range(0, (feldervertikal + 1) * kaestchengroesse, kaestchengroesse):
        pygame.draw.line(bildschirm, schwarz, (60, y + 120), (460, y + 120))
    pygame.display.update()

'''Main-Schleife damit das Spiel durchgehend laufen kann'''
go = True
while go:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()
    spielfeld_erstellen()
