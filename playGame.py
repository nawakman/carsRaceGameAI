"""
code by Theo Pernel,INSA Rennes, 2025
"""

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

        self.maxSpeed=10
        self.playerSpeed=0
        self.playerDirection=pygame.math.Vector2(0,0)
        self.crashLocations=set()#set for no duplicates

        pygame.init()
        pygame.display.set_caption("cars racing game")

        monitorInfo=pygame.display.Info()
        self.screenResolution=(monitorInfo.current_w,monitorInfo.current_h)#get hardware monitor resolution #must be set before reading the files
        
        self.LoadMap(mapPictureFile,mapResolution)#this function will set self.windowResolution used by next function
        self.screen=pygame.display.set_mode(self.windowResolution)
   
    def Play(self,mode="alone",AIFile=""):#3 modes, "alone"/"vsAI"/"AI"
        self.mode=mode
        self.currentTurn=1#first turn is 1 not 0
        match mode:
            case "alone":
                self.PlayAlone()
            case "vsAI":
                self.LoadAI(AIFile)
                self.PlayAgainstAI()
            case "AI":
                self.LoadAI(AIFile)
                self.VisualizeGeneration()

    def PlayAlone(self):
        while True:
            #EVENTS
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    quit()
            #INPUTS
            self.UpdatePlayerInputs()
            #GAMEPLAY
            if self.mouseState[0]:#left click
                self.PlayerMove()
                if self.IsFinishLine(self.playerMoves[-1]):
                    print("player won")
                pygame.time.delay(150)
            #RENDER
            self.Render()       

    def PlayAgainstAI(self):
        self.playerTurn=True

        while True:
            #EVENTS
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    quit()
            #INPUTS
            self.UpdatePlayerInputs()

            #GAMEPLAY
            if self.playerTurn:
                if self.mouseState[0]:#left click
                    self.PlayerMove()

                    if self.IsFinishLine(self.playerMoves[-1]):
                        print("player won")

                    pygame.time.delay(150)
                    self.playerTurn=False
            else:
                if len(self.AIMoves[0])>self.currentTurn:#if AI still have moves
                    if self.IsFinishLine(self.AIMoves[0][self.currentTurn]):
                        print("AI won")
                else:
                    print("AI has no more moves, cannot reach finish line") 
                self.playerTurn=True
                self.currentTurn+=1
            #RENDER
            self.Render()

    def VisualizeGeneration(self):#it only display AI Moves, they can be wrong if the data in the file is wrong 
        while True:
            #EVENTS
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    quit()
            #GAMEPLAY
            pygame.time.delay(50)#display a new move each x milliseconds
            self.currentTurn+=1
            #print(self.currentTurn)
            #RENDER
            self.Render()

    def LoadMap(self,mapPictureFile,mapResolution):
        self.mapMaker=pictureToCircuit(mapPictureFile,mapResolution)

        self.mapName=mapPictureFile.split(".")[0]
        tileFilePath="circuits/"+self.mapName+".txt"
        pointsFilePath="circuits/"+self.mapName+"Points.txt"
        if not os.path.isfile(tileFilePath):#if map text files does not already exists then generate
            self.mapMaker.CreateCircuitFromPicture()#generates needed text files for next functions
        self.ReadTileMapFromFile(tileFilePath)
        self.ReadPointsFromFile(pointsFilePath)

        if(mapResolution!=self.mapResolutionFromFile):#if map exist but mapResolution has been changed since then regenerate
            self.mapMaker.CreateCircuitFromPicture()
            self.ReadTileMapFromFile(tileFilePath)#refresh map since it has been regenerated
            self.ReadPointsFromFile(pointsFilePath)

    def LoadAI(self,AIFile):#mapName in first line #nb individuals in second line
        self.AIMoves=[]#will look like [[ai1][ai2][ai3],...] and each "aiX" will look like [[x1,y1],[x2,y2],[x3,y3],...]
        self.segmentThatCrash=[]#list of dict of position:position

        filePath="AI/"+AIFile
        if not os.path.isfile(filePath):#if map text files does not already exists then generate
            print("AI file"+filePath+" does not exist")
            quit()
        with open(filePath) as file:
            mapNameFromFile=file.readline().rstrip("\n")#first line contains the name of the map the AI was trained on
            if not self.mapName==mapNameFromFile:#maps need to match
                print("This AI was trained on: "+mapNameFromFile+" but you loaded: "+self.mapName)
                quit()

            nbIndividuals=int(file.readline())
            for i in range(nbIndividuals):
                ###CORRECT AI MOVE###
                line=file.readline()
                individualMoves=[]#temporary container to store all moves of an individual
                coordinates=line.split(";")

                for strCoord in coordinates:
                    coord=[int(x) for x in strCoord.split(",")]
                    coord[0],coord[1]=coord[1],coord[0]#in the c program coord are (line, column), in this script it is (column, line)
                    coord=pygame.math.Vector2(coord)#convert to vector2
                    individualMoves.append(coord)
                self.AIMoves.append(individualMoves)#one list per player

                ###TO DISPLAY A RED DOT WHERE AI CRASHED###
                line=file.readline()
                individualSegmentThatCrash={}#temporary container to store all moves of an individual
                coordinates=line.split(";")

                for strCoord in coordinates:
                    coord=[int(x) for x in strCoord.split(",")]
                    fromCoord=(int(coord[1]),int(coord[0]))#in c++ we choose coordinates as i,j but here we use x,y
                    #fromCoord=pygame.math.Vector2(fromCoord)#convert to vector2 #BAD VECTOR2 CANNOT BE HASHED
                    toCoord=(int(coord[3]),int(coord[2]))#in c++ we choose coordinates as i,j but here we use x,y
                    #toCoord=pygame.math.Vector2(toCoord)#convert to vector2
                    individualSegmentThatCrash[fromCoord]=toCoord

                self.segmentThatCrash.append(individualSegmentThatCrash)#one dictionnary per player
        
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
                        #self.AIMoves=[self.start] #when we'll read AI moves from file it will already start at start
                    elif line[x]=="e":
                        self.end=pygame.math.Vector2(x,y)

    def Render(self):
        self.DrawMap()
        self.DrawPoints()
        self.DrawCrashes()
        match self.mode:
            case "alone":
                self.DrawMoves(self.playerMoves,self.playerMoveColor)
                self.DrawNextPlayerMove()
            case "vsAI":
                self.DrawMoves(self.playerMoves,self.playerMoveColor)
                self.DrawNextPlayerMove()
                limitIfNoMoveLeft=min(self.currentTurn,len(self.AIMoves[0]))
                self.DrawMoves(self.AIMoves[0][:limitIfNoMoveLeft],(100,100,0))#AI are stored from best to worst #only display moves up to the current turn
                thisMove=self.AIMoves[0][min(self.currentTurn,len(ai)-1)]#retrieve 1 because we slices end coord are +1 but index is not
                self.CheckIfAICrashed(thisMove,0)
            case "AI":
                for i in range(len(self.AIMoves)):
                    ai=self.AIMoves[i]
                    limitIfNoMoveLeft=min(self.currentTurn,len(ai))
                    self.DrawMoves(ai[:limitIfNoMoveLeft],(100,100,0))#if an ai finished faster it will have less moves #only draw up to current turn
                    thisMove=ai[min(self.currentTurn,len(ai)-1)]#retrieve 1 because we slices end coord are +1 but index is not
                    self.CheckIfAICrashed(thisMove,i)

        pygame.display.update()
        self.clock.tick(60)#60fps

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

    def DrawMoves(self,movesArray,color):
        for i in range(len(movesArray)-1):
            pygame.draw.line(self.screen,color,movesArray[i]*self.tileSize,movesArray[i+1]*self.tileSize,width=self.lineWidth)
            pygame.draw.circle(self.screen,color,movesArray[i+1]*self.tileSize,3)

    def DrawCrashes(self):
        for crash in self.crashLocations:
            pygame.draw.circle(self.screen,(255,0,0),pygame.math.Vector2(crash)*self.tileSize,5)

    def DrawNextPlayerMove(self):
        thisMove,speed,direction=self.PossiblePlayerMove()
        thisMove*=self.tileSize#scale from grid position to screen position
        lastMove=pygame.math.Vector2(self.playerMoves[-1])*self.tileSize
        pygame.draw.line(self.screen,self.playerMoveColor,lastMove,thisMove,width=self.lineWidth)

    def UpdatePlayerInputs(self):
        self.mousePosition=pygame.mouse.get_pos()
        avoidFloorSnapping=self.tileSize/2
        self.mousePosition=((self.mousePosition[0]+avoidFloorSnapping)//self.tileSize,(self.mousePosition[1]+avoidFloorSnapping)//self.tileSize)#convert mouse position to the nearest grid position instead of the smaller grid position
        self.mouseState=pygame.mouse.get_pressed()

    def PossiblePlayerMove(self):
        mouseDirection=pygame.math.Vector2(self.mousePosition)-pygame.math.Vector2(self.playerMoves[-1])#vector from last player position to mouse
        avoidFloorSnapping=45/2#for now the angle snaps to the smallest division of 45° insted of the nearest
        #e.g. we to go up, we want to snap to 90 degrees when cursor in the 67.5-112.5 range, but for now it snap when cursor is in 90-135 range
        angle=((round(mouseDirection.angle_to(pygame.math.Vector2(1,0))+avoidFloorSnapping)//45)*45)#angle from mouse to horizontal with origin at last player position #round to nearest 45°

        minSpeed=max(1,self.playerSpeed-1)
        maxSpeed=min(self.maxSpeed,self.playerSpeed+1)if self.playerDirection.dot(mouseDirection)>=0 else minSpeed#if mouse is not oriented in the player direction we want the minSpeed

        if angle%90==0:
            speed=Clamp(round(mouseDirection.magnitude()),minSpeed,maxSpeed)#we can either accelerate stay same speed or decelerate
        else:#we deduct speed from a distance,if we go diagonally distance is longer and speed is scaled by an additional \/‾2, in this game a diagonal move cost only 1
            speed=Clamp(round(mouseDirection.magnitude()/sqrt(2)),minSpeed,maxSpeed)#we can either accelerate stay same speed or decelerate

        if self.playerDirection!=[0,0]:#if [0,0] all directions allowed, else restrict to 45° turn at most
            playerAngle=((round(self.playerDirection.angle_to(pygame.math.Vector2(1,0)))//45)*45)#get angle compared to horizontal #round to nearest 45°
            angle=ClampToNearestAngle(angle,playerAngle-45,playerAngle+45)

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

    def PlayerMove(self):#first get the actual move, then apply it
        position,speed,direction=self.PossiblePlayerMove()
        self.MovePlayer(speed,direction)

    def MovePlayer(self,newSpeed,newDirection):#scan each tile from last position to final position to check if a crash happened
        scannedTile=self.playerMoves[-1].copy()#we don't want to change the player move from the array
        
        for i in range(newSpeed):
            if self.pointMatrix[int((scannedTile+newDirection)[0])][int((scannedTile+newDirection)[1])]=="n":#if next move will crash in wall then put player just before the wall
                isMoveDiagonal=newDirection.angle_to(pygame.math.Vector2(1,0))%90!=0#if moving diagonally
                crashPosition=scannedTile+newDirection*(0.5 if isMoveDiagonal and self.IsDiagonalTile(scannedTile+newDirection) else 1)#if we crash in a diagonal tile we need to put the point on the diagonal wall not on the tile behind
                self.crashLocations.add((crashPosition[0],crashPosition[1]))#the set will remove duplicates by itself #convert to tuple because Vector2 i not hashable for set
                
                #if a crash is detected then the actual scannedTile is the position right before the wall
                self.playerSpeed=0
                self.playerDirection=pygame.math.Vector2(0,0)
                if scannedTile!=self.playerMoves[-1]:#if two moves are in the same position game crashes (division by 0 I guess)
                    self.playerMoves.append(scannedTile)
                return
            else:
                scannedTile=scannedTile+newDirection#continue checking the path
        
        #if no crah were detected, scannedTile is now at the good position
        self.playerSpeed=newSpeed
        self.playerDirection=newDirection
        self.playerMoves.append(scannedTile)   
        
    def IsFinishLine(self, position):
        return self.pointMatrix[int(position[0])][int(position[1])]=="e"
        
    def IsDiagonalTile(self, position):
        accessibleCount=0
        for x in range(-1,2):
            for y in range(-1,2):
                accessibleCount+=1 if self.pointMatrix[int(position[0])+x][int(position[1])+y]=="y" else 0
        return accessibleCount==1

    def CheckIfAICrashed(self,move,ai_id):
        move=(int(move[0]),int(move[1]))
        if move in self.segmentThatCrash[ai_id].keys():#if this move caused a crash
            moveEnd=self.segmentThatCrash[ai_id][move]
            moveDirection=pygame.math.Vector2(moveEnd)-pygame.math.Vector2(move)
            isMoveDiagonal=moveDirection.angle_to(pygame.math.Vector2(1,0))%90!=0#if moving diagonally
            crashPosition=move+moveDirection*(0.5 if isMoveDiagonal and self.IsDiagonalTile(move+moveDirection) else 1)#if we crash in a diagonal tile we need to put the point on the diagonal wall not on the tile behind
            self.crashLocations.add((crashPosition[0],crashPosition[1]))#the set will remove duplicates by itself #convert to tuple because Vector2 i not hashable for set

def Clamp(x, minValue, maxValue):
    return max(min(x, maxValue), minValue)

def Limit(x,limit1,limit2):#clamp but value can be inversed at runtime
    if limit1<=limit2:
        return Clamp(x,limit1,limit2)
    else:
        return Clamp(x,limit2,limit1)

def ClampToNearestAngle(angle,minAngle,maxAngle):#clamp angle #angle restriction can range 0-360 #trigonometric direction
    angleMin=From360To180Range(minAngle%360)
    angleMax=From360To180Range(maxAngle%360)
    a=From360To180Range(angle%360)

    if angleMax<angleMin:#restriction area passs through the problematic -180=180 area
        if angleMax<=a<=angleMin:#angle is in the problematic -180=180 area 
            distanceToMin=minAngleDistance(a,angleMin)
            distanceToMax=minAngleDistance(a,angleMax)
            #print("___\n",distanceToMin,distanceToMax)
            return angleMin%360 if distanceToMin<=distanceToMax else angleMax%360
        else:#angle is not in the problematic -180=180 area
            return a%360
          
    else:#restriction area doe not passs through the problematic -180=180 area #UNDER THIS I OK
        if angleMin<=a<=angleMax:#angle is not in the problematic -180=180 area 
            return a%360
        else:#angle is in the problematic -180=180 area
            distanceToMin=minAngleDistance(a,angleMin)
            distanceToMax=minAngleDistance(a,angleMax)
            #print("___\n",distanceToMin,distanceToMax)
            return angleMin%360 if distanceToMin<=distanceToMax else angleMax%360

def ClampToNearestAngle2(alpha,A1,A2):#alternative version #exactly same behaviour #another approach
    if (alpha<0):
        alpha=alpha%360
    if (A1<0):
        A1=A1%360
    if (A2<0):
        A2=A2%360
    if (A2>A1):
        if (alpha<=A2 and alpha>=A1):
            return alpha
        # Calcul de A3 
        t=(360-(A2-A1))/2
        if (A2+t <= 360):
            A3 = A2 + t
        else:
            A3 = A1 - t
        # Redirection
        #print("A1, A2, A3, t")
        #print(A1,A2,A3,t)
        if (A3>=180):
            if (alpha<A3 and alpha>=A2):
                return A2
            else:
                return A1
        else:
            if (alpha>A3 and alpha<=A1):
                return A1
            else:
                return A2

    elif(A1>A2):
        if ((alpha<=A2 and alpha>=0) or (alpha>=A1 and alpha<=360)):
            return alpha
        # Calcul A3
        t=(A1-A2)/2
        A3=A2+t
        # Redirection
        #print("A1, A2, A3, t")
        #print(A1,A2,A3,t)
        if (alpha<A3):
            return A2
        return A1

def minAngleDistance(fromAngle,toAngle):
    fa=From360To180Range(fromAngle%360)
    ta=From360To180Range(toAngle%360)
    if ta<fa:
        fa,ta=ta,fa
    return min(abs(ta-fa),abs(180-ta)+abs(-180-fa))#return minimum distance, either from (fa to ta) or from (fa to 180 and -180 to ta)

def From360To180Range(angle):#from 0,360 to -180,180
    return angle if angle<=180 else angle-360

game=carsRace("test1.png",10,0.75)
#game.Play("alone")
#game.Play("vsAI","../AI/test1-gen0.txt")
game.Play("AI","../AI/test1-gen2.txt")