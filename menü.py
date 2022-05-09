import pygame, sys

clock = pygame.time.Clock()
from pygame.locals import *

pygame.init()
pygame.display.set_caption('Schiffe versenken')
bildschirm = pygame.display.set_mode((500, 500), 0, 32)
hintergrund = pygame.image.load("Grafiken-SchiffeVersenken/menü_hintergund.jpg")

font = pygame.font.SysFont(None, 50)
font2 = pygame.font.SysFont(None, 35)

def draw_text(text, font, color, surface, x, y):
    textobj = font.render(text, 1, color)
    textrect = textobj.get_rect()
    textrect.topleft = (x, y)
    surface.blit(textobj, textrect)


click = False


def main_menu():
    while True:

        bildschirm.blit(hintergrund, (0,0))
        draw_text('Hauptmenü', font, (0, 0, 0), bildschirm, 20, 20)

        mx, my = pygame.mouse.get_pos()

        button_1 = pygame.Rect(50, 100, 200, 50)
        button_2 = pygame.Rect(50, 200, 200, 50)
        button_3 = pygame.Rect(50, 300, 200, 50)
        button_4 = pygame.Rect(50, 400, 200, 50)
        draw_text('Einzelspieler', font2, (0, 0, 0), bildschirm, 50, 110)
        draw_text('Mehrspieler', font2, (0, 0, 0), bildschirm, 50, 210)
        draw_text('Einstellungen', font2, (0, 0, 0), bildschirm, 50, 310)
        draw_text('Exit', font2, (0, 0, 0), bildschirm, 50, 410)
        pygame.display.update()


        if button_1.collidepoint((mx, my)):
            if click:
                einzelspieler()
        if button_2.collidepoint((mx, my)):
            if click:
                mehrspieler()
        if button_3.collidepoint((mx, my)):
            if click:
                einstellungen()
        if button_4.collidepoint((mx, my)):
            if click:
                exit()
        pygame.draw.rect(bildschirm, (255, 255, 255), button_1)
        pygame.draw.rect(bildschirm, (255, 255, 255), button_2)
        pygame.draw.rect(bildschirm, (255, 255, 255), button_3)
        pygame.draw.rect(bildschirm, (255, 255, 255), button_4)

        click = False
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                sys.exit()
            if event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    pygame.quit()
                    sys.exit()
            if event.type == MOUSEBUTTONDOWN:
                if event.button == 1:
                    click = True

        pygame.display.update()
        clock.tick(60)


def einzelspieler():
    running = True
    while running:
        bildschirm.blit(hintergrund, (0, 0))

        draw_text('Einzelspieler', font, (0, 0, 0), bildschirm, 20, 20)
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                sys.exit()
            if event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    running = False

        pygame.display.update()
        clock.tick(60)


def mehrspieler():
    running = True
    while running:
        bildschirm.blit(hintergrund, (0, 0))

        draw_text('Mehrspieler', font, (0, 0, 0), bildschirm, 20, 20)
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                sys.exit()
            if event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    running = False

        pygame.display.update()
        clock.tick(60)


def einstellungen():
    running = True
    while running:
        bildschirm.blit(hintergrund, (0, 0))

        draw_text('EInstellungen', font, (0, 0, 0), bildschirm, 20, 20)
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
                sys.exit()
            if event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    running = False

        pygame.display.update()
        clock.tick(60)


def exit():
    pygame.quit()
    sys.exit()
    pygame.display.update()
    clock.tick(60)


main_menu()
