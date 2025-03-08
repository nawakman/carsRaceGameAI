import os.path
import pygame
from time import sleep
from pictureToCircuit import pictureToCircuit

class carsRace:
    def __init__(self,mapPictureFile,mapResolution,windowScaleFactor=1):
        #mapResolution is how much you want the map to ressemble the image(low=big map and ressemble picture/high=small map approximated to big chunks)
        self.windowScaleFactor=windowScaleFactor
        self.backgroundColor=(255,255,255)
        self.wallColor=(0,0,0)

        pygame.init()

        monitorInfo=pygame.display.Info()
        self.screenResolution=(monitorInfo.current_w,monitorInfo.current_h)#get hardware monitor resolution #must ne set before reading the files
        pygame.display.set_caption("cars racing game")
        mapMaker=pictureToCircuit(mapPictureFile,mapResolution)

        textFileName=mapPictureFile.split(".")[0]+".txt"
        if not os.path.isfile(textFileName):#if map text files does not already exists then generate
            mapMaker.CreateCircuitFromPicture()#generates needed text files for next functions
        self.ReadMapFromFile(textFileName)

        if(mapResolution!=self.mapResolutionFromFile):#if map exist but mapResolution has been changed since then regenerate
            mapMaker.CreateCircuitFromPicture()
            self.ReadMapFromFile(textFileName)#refresh map since it has been regenerated

        self.screen=pygame.display.set_mode(self.windowResolution)
        #pictureToCircuit.PrintMatrix(self.tileMatrix)

        while True:#game loop

            # event loop
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    quit()
            

            self.DrawMap()
            pygame.display.update()
            sleep(1/60)#60fps

    def ReadMapFromFile(self,fileName):
        with open("circuits/"+fileName) as file:
            self.matrixSize=[int(x) for x in file.readline().split(";")]#parse first line to int
            self.mapResolutionFromFile=int(file.readline().split("=")[1])#read map resolution from file()

            #next 5 lines adjust the map and window size to display any size of map properly without exiting the screen
            if self.matrixSize[0]<self.matrixSize[1]:#if taller than wider
                self.tileSize=int((self.screenResolution[1]//self.matrixSize[1])*self.windowScaleFactor)#width is limiting factor #scale by window scale factor
            else:
                self.tileSize=int((self.screenResolution[0]//self.matrixSize[0])*self.windowScaleFactor)#height is limiting factor #scale by window scale factor

            self.windowResolution=(self.matrixSize[0]*self.tileSize,self.matrixSize[1]*self.tileSize)#adjust window size depending on map size and screen size

            self.tileMatrix=[[""for y in range(self.matrixSize[1])] for x in range(self.matrixSize[0])]# a "#" means wall
            for y in range(self.matrixSize[1]):
                line=file.readline()
                for x in range(self.matrixSize[0]):
                    self.tileMatrix[x][y]=line[x]

    def DrawMap(self):
        self.screen.fill(self.backgroundColor)
        #pygame.draw.rect(self.screen,self.backgroundColor,pygame.Rect(0,0,self.screenResolution[0],self.screenResolution[1]))
        for x in range(self.matrixSize[0]):
            for y in range(self.matrixSize[1]):
                if self.tileMatrix[x][y]=="#":
                    pygame.draw.rect(self.screen,self.wallColor,pygame.Rect(x*self.tileSize,y*self.tileSize,self.tileSize,self.tileSize))
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

game=carsRace("test2.png",12,0.75)