import os.path
import pygame
from time import sleep
from pictureToCircuit import *

class carsRace:
    def __init__(self,mapPictureFile,mapResolution,windowScaleFactor=1):
        #mapResolution is how much you want the map to ressemble the image(low=big map and ressemble picture/high=small map approximated to big chunks)
        self.windowScaleFactor=windowScaleFactor
        self.backgroundColor=(255,255,255)
        self.wallColor=(0,0,0)

        pygame.init()

        monitorInfo=pygame.display.Info()
        self.screenResolution=(monitorInfo.current_w,monitorInfo.current_h)#get hardware monitor resolution #must be set before reading the files
        pygame.display.set_caption("cars racing game")
        self.mapMaker=pictureToCircuit(mapPictureFile,mapResolution)

        tileFilePath="circuits/"+mapPictureFile.split(".")[0]+".txt"
        pointsFilePath="circuits/"+mapPictureFile.split(".")[0]+"Points.txt"
        if not os.path.isfile(tileFilePath):#if map text files does not already exists then generate
            self.mapMaker.CreateCircuitFromPicture()#generates needed text files for next functions
        self.ReadTileMapFromFile(tileFilePath)
        self.ReadPointsFromFile(pointsFilePath)

        if(mapResolution!=self.mapResolutionFromFile):#if map exist but mapResolution has been changed since then regenerate
            self.mapMaker.CreateCircuitFromPicture()
            self.ReadTileMapFromFile(tileFilePath)#refresh map since it has been regenerated
            self.ReadPointsFromFile(pointsFilePath)

        self.screen=pygame.display.set_mode(self.windowResolution)

        #GAME LOOP
        while True:
            # event loop
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    quit()
            

            self.DrawMap()
            self.DrawPoints()
            pygame.display.update()
            sleep(1/60)#60fps

    def ReadTileMapFromFile(self,filePath):
        with open(filePath) as file:
            self.tileMatrixSize=[int(x) for x in file.readline().split(";")]#parse first line to int
            self.mapResolutionFromFile=int(file.readline().split("=")[1])#read map resolution from file()

            #next 5 lines adjust the map and window size to display any size of map properly without exiting the screen
            if self.tileMatrixSize[0]<=self.tileMatrixSize[1]:#if taller than wider
                self.tileSize=round((self.screenResolution[1]/self.tileMatrixSize[1])*self.windowScaleFactor)#width is limiting factor #scale by window scale factor
            else:
                self.tileSize=round((self.screenResolution[0]/self.tileMatrixSize[0])*self.windowScaleFactor)#height is limiting factor #scale by window scale factor

            self.windowResolution=(self.tileMatrixSize[0]*self.tileSize,self.tileMatrixSize[1]*self.tileSize)#adjust window size depending on map size and screen size

            self.tileMatrix=[[""for y in range(self.tileMatrixSize[1])] for x in range(self.tileMatrixSize[0])]# a "#" means wall
            for y in range(self.tileMatrixSize[1]):
                line=file.readline()
                for x in range(self.tileMatrixSize[0]):
                    self.tileMatrix[x][y]=line[x]
        
    def ReadPointsFromFile(self,filePath):
        with open(filePath) as file:
            self.pointMatrixSize=[int(x) for x in file.readline().split(";")]#parse first line to int
            self.pointMatrix=[[""for y in range(self.pointMatrixSize[1])] for x in range(self.pointMatrixSize[0])]# a "a" means accessible
            for y in range(self.pointMatrixSize[1]):
                line=file.readline()
                for x in range(self.pointMatrixSize[0]):
                    self.pointMatrix[x][y]=line[x]

    def DrawMap(self):
        self.screen.fill(self.backgroundColor)
        for x in range(self.tileMatrixSize[0]):
            for y in range(self.tileMatrixSize[1]):
                if self.tileMatrix[x][y]=="#":
                    #draw rect also works but does not align perfectly(causing unpleasant visual artifacts) with the shape generated with draw polygon, so we only use draw polygon 
                    #pygame.draw.rect(self.screen,self.wallColor,pygame.Rect(x*self.tileSize,y*self.tileSize,self.tileSize,self.tileSize))
                    pygame.draw.polygon(self.screen,self.wallColor,((x*self.tileSize,y*self.tileSize),((x+1)*self.tileSize,y*self.tileSize),((x+1)*self.tileSize,(y+1)*self.tileSize),(x*self.tileSize,(y+1)*self.tileSize)))
        self.DrawAngles()

    def DrawAngles(self):
        """we sample 4 values around the scanned pixels a put it in a string in the following order
         _ _
        |1|2|
        |3|4| corresponds to "1234"
        we then use a lookup table to get the point of the polygon to draw
         """
        u=self.tileSize#unit #for readability
        tileToPolygonLUT={#describe the shape needed to make
        "### ": ((u,u),(2*u,u),(u,2*u)),
        "## #": ((0,u),(u,u),(u,2*u)),
        " ###": ((u,0),(u,u),(0,u)),
        "# ##": ((u,0),(2*u,u),(u,u)),
        "#  #": ((u,0),(2*u,u),(u,2*u),(0,u)),
        " ## ": ((u,0),(2*u,u),(u,2*u),(0,u))#it is normal that the two last have the same polygon points
        }
        for x in range(self.tileMatrixSize[0]-1):
            for y in range(self.tileMatrixSize[1]-1):
                key=self.tileMatrix[x][y]+self.tileMatrix[x+1][y]+self.tileMatrix[x][y+1]+self.tileMatrix[x+1][y+1]
                if key in tileToPolygonLUT.keys():
                    adjustToCoordinates=[(coord[0]+x*self.tileSize,coord[1]+y*self.tileSize) for coord in tileToPolygonLUT[key]]
                    pygame.draw.polygon(self.screen,self.wallColor,adjustToCoordinates)

    def DrawPoints(self):
        pointColor=(0,255,0)
        startPointColor=(0,200,0)
        endPointColor=(200,0,0)
        for x in range(self.pointMatrixSize[0]):
            for y in range(self.pointMatrixSize[1]):
                if self.pointMatrix[x][y]=="y":#if tile accessible
                    pygame.draw.circle(self.screen,pointColor,(x*self.tileSize,y*self.tileSize),2)
                elif self.pointMatrix[x][y]=="s":
                    pygame.draw.circle(self.screen,startPointColor,(x*self.tileSize,y*self.tileSize),8)
                elif self.pointMatrix[x][y]=="e":
                    pygame.draw.circle(self.screen,endPointColor,(x*self.tileSize,y*self.tileSize),8)

game=carsRace("testCircuit.png",10,0.75)