import os.path
import pygame
from math import*
from pictureToCircuit import *

class carsRace:
    def __init__(self,mapPictureFile,mapResolution,windowScaleFactor=1):
        """
        mapResolution is how much you want the map to ressemble the image
        (low=big map and ressemble picture/high=small map approximated to big chunks)

        windowScaleFactor is the percentage of the screen the window will take
        (1=max size, fill screen either vertically or horizontally depending on map, 0-1=smaller)
        """
        self.windowScaleFactor=windowScaleFactor
        self.lineWidth=3
        self.backgroundColor=(255,255,255)
        self.wallColor=(0,0,0)
        self.playerMoveColor=(0,0,220)
        self.clock=pygame.time.Clock()
        self.playerTurn=True

        self.maxSpeed=10
        self.playerSpeed=0
        self.playerDirection=pygame.math.Vector2(0,0)

        pygame.init()
        pygame.display.set_caption("cars racing game")

        monitorInfo=pygame.display.Info()
        self.screenResolution=(monitorInfo.current_w,monitorInfo.current_h)#get hardware monitor resolution #must be set before reading the files
        
        self.LoadMap(mapPictureFile,mapResolution)#this function will set self.windowResolution used by next function
        self.screen=pygame.display.set_mode(self.windowResolution)

        #GAME LOOP
        while True:
            #EVENTS
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    quit()
            #INPUTS
            self.mousePosition=pygame.mouse.get_pos()
            avoidFloorSnapping=self.tileSize/2
            self.mousePosition=((self.mousePosition[0]+avoidFloorSnapping)//self.tileSize,(self.mousePosition[1]+avoidFloorSnapping)//self.tileSize)#convert mouse position to the nearest grid position instead of the smaller grid position
            self.mouseState=pygame.mouse.get_pressed()
            #GAMEPLAY
            if self.playerTurn:
                if self.mouseState[0]:#left click
                    self.PlayerMove()
                    self.playerTurn=False
            else:
                self.AskAIMove()
                self.playerTurn=True   
            #RENDER
            self.DrawMap()
            self.DrawPoints()
            self.DrawMoves(self.playerMoves,self.playerMoveColor)
            self.DrawNextPlayerMove()
            self.DrawMoves(self.AIMoves,(100,100,0))

            pygame.display.update()
            self.clock.tick(60)#60fps

    def LoadMap(self,mapPictureFile,mapResolution):
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
                    if line[x]=="s":
                        self.start=pygame.math.Vector2(x,y)
                        self.playerMoves=[self.start]
                        self.AIMoves=[self.start]
                    elif line[x]=="e":
                        self.end=pygame.math.Vector2(x,y)

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
    
    def PlayerMove(self):
        thisMove,speed,direction=self.PossiblePlayerMove()
        if self.pointMatrix[int(thisMove[0])][int(thisMove[1])]=="n":#crashed into a wall #vector2 convert components to float, so we put it bak to int
            self.playerMoves.append(self.UncrashPlayer(thisMove))
            self.speed=0
            self.playerDirection=pygame.math.Vector2(0,0)
        else:
            self.playerMoves.append(thisMove)
            self.playerSpeed=speed
            self.playerDirection=direction
    
    def UncrashPlayer(self,lastMove):
        quit()
        pass

    def AskAIMove(self):
        pygame.time.delay(150)#WIP, replace with actual AI program

    def DrawMoves(self,movesArray,color):
        for i in range(len(movesArray)-1):
            pygame.draw.line(self.screen,color,movesArray[i]*self.tileSize,movesArray[i+1]*self.tileSize,width=self.lineWidth)
            pygame.draw.circle(self.screen,color,movesArray[i+1]*self.tileSize,3)
    
    def DrawNextPlayerMove(self):
        thisMove,speed,direction=self.PossiblePlayerMove()
        thisMove*=self.tileSize#scale from grid position to screen position
        lastMove=pygame.math.Vector2(self.playerMoves[-1])*self.tileSize
        pygame.draw.line(self.screen,self.playerMoveColor,lastMove,thisMove,width=self.lineWidth)

    def PossiblePlayerMove(self):
        mouseDirection=pygame.math.Vector2(self.mousePosition)-pygame.math.Vector2(self.playerMoves[-1])#vector from last player position to mouse
        avoidFloorSnapping=45/2#for now the angle snaps to the smallest division of 45° insted of the nearest
        #e.g. we to go up, we want to snap to 90 degrees when cursor in the 67.5-112.5 range, but for now it snap when cursor is in 90-135 range
        angle=((round(mouseDirection.angle_to(pygame.math.Vector2(1,0))+avoidFloorSnapping)//45)*45)#angle from mouse to horizontal with origin at last player position #round to nearest 45°

        minSpeed=max(1,self.playerSpeed-1)
        maxSpeed=min(self.maxSpeed,self.playerSpeed+1)if self.playerDirection.dot(mouseDirection)>=0 else minSpeed#if mouse is not oriented in the player direction we don't want the minSpeed

        if angle%90==0:
            speed=Clamp(round(mouseDirection.magnitude()),minSpeed,maxSpeed)#we can either accelerate stay same speed or decelerate
        else:#we deduct speed from a distance,if we go diagonally distance is longer and speed is scaled by an additional \/‾2, in this game a diagonal move cost only 1
            speed=Clamp(round(mouseDirection.magnitude()/sqrt(2)),minSpeed,maxSpeed)#we can either accelerate stay same speed or decelerate

        if self.playerDirection!=[0,0]:#if [0,0] all directions allowed, else restrict to 45° turn at most
            playerAngle=((round(self.playerDirection.angle_to(pygame.math.Vector2(1,0)))//45)*45)#get angle compared to horizontal #round to nearest 45°
            angle=Clamp(angle,playerAngle-45,playerAngle+45)#restrict moves to 45° turn at most

        angle=angle%360#do this only once at the end

        match angle:#NOTE: origin is top left so Y axis is reversed
            case 0:
                result=pygame.math.Vector2(1,0)
            case 45:
                result=pygame.math.Vector2(1,-1)
            case 90:
                result=pygame.math.Vector2(0,-1)
            case 135:
                result=pygame.math.Vector2(-1,-1)
            case 180:
                result=pygame.math.Vector2(-1,0)
            case 225    :
                result=pygame.math.Vector2(-1,1)
            case 270:
                result=pygame.math.Vector2(0,1)
            case 315:
                result=pygame.math.Vector2(1,1)
        return result*speed+self.playerMoves[-1],speed,result#convert from local coordinates to world coordinates
        
def Clamp(n, min_value, max_value):
    return max(min(n, max_value), min_value)

def Limit(n,limit1,limit2):#clamp but value can be inversed at runtime
    if limit1<=limit2:
        return Clamp(n,limit1,limit2)
    else:
        return Clamp(n,limit2,limit1)

game=carsRace("testCircuit.png",10,0.75)