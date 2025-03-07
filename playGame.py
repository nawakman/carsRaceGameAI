import pygame
import pictureToCircuit

class carsRace:
    def __init__(self,screenResolution,windowScaleFactor=1):
        self.backgroundColor=(255,255,255)
        self.wallColor=(0,0,0)

        pygame.init()
        #pygame.display.set_icon(pygame.image.load("logo.jpg"))
        pygame.display.set_caption("cars racing game")
        self.screenResolution=screenResolution
        self.windowScaleFactor=windowScaleFactor
        self.ReadMapFromFile("testCircuit.txt")
        self.screen= pygame.display.set_mode(self.windowResolution) 
        #pictureToCircuit.PrintMatrix(self.tileMatrix)

        while True:
            self.DrawMap()
            pygame.display.update()

    def ReadMapFromFile(self,fileName):
        with open(fileName) as file:
            self.matrixSize=[int(x) for x in file.readline().split(";")]#parse first line to int

            self.tilesize=int((min(self.screenResolution)//max(self.matrixSize))*self.windowScaleFactor)#adjust size to screen and scale size by factor#floor to avoid visual artifacts
            self.windowResolution=(min(self.screenResolution[0],self.matrixSize[0]*self.tilesize),min(self.screenResolution[1],self.matrixSize[1]*self.tilesize))#adjust window size depending on map and screen size

            self.tileMatrix=[[""for y in range(self.matrixSize[1])] for x in range(self.matrixSize[0])]# a "#" means wall
            for y in range(self.matrixSize[1]):
                line=file.readline()
                for x in range(self.matrixSize[0]):
                    self.tileMatrix[x][y]=line[x]

    def DrawMap(self):
        pygame.draw.rect(self.screen,self.backgroundColor,pygame.Rect(0,0,self.screenResolution[0],self.screenResolution[1]))
        for x in range(self.matrixSize[0]):
            for y in range(self.matrixSize[1]):
                if self.tileMatrix[x][y]=="#":
                    pygame.draw.rect(self.screen,self.wallColor,pygame.Rect(x*self.tilesize,y*self.tilesize,self.tilesize,self.tilesize))
        self.DrawAngles()

    def DrawAngles(self):
        """we sample 4 values around the scanned pixels a put it in a string in the following order
         _ _
        |1|2|
        |3|4| corresponds to "1234"
         ‾ ‾
        then we use a marching square like algorithm
         """
        tileToAngleLUT={
    "### ": "1",
    "## #": "1",
    " ###": "1",
    "# ##": "1",
    "#  #": "1",
    " ## ": "1",
    }
        for x in range(self.matrixSize[0]-1):
            for y in range(self.matrixSize[1]-1):
                key=self.tileMatrix[x][y]+self.tileMatrix[x+1][y]+self.tileMatrix[x][y+1]+self.tileMatrix[x+1][y+1]
                if key in tileToAngleLUT.keys():
                    #pygame.draw.rect(self.screen,self.wallColor,pygame.Rect())
                    pass

game=carsRace((1920,1080),0.75)