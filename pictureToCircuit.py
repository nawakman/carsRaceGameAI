from PIL import Image

class pictureToCircuit:
    def __init__(self,fileName,tileSize):#tilesize is how many pixel per tile #e.g. 512*512px picture with tilesize 16 will result in 32*32 tiles map
        self.fileName=fileName
        self.picture=Image.open(fileName,"r")
        width,height=self.picture.size
        self.tileSize=tileSize
        self.nbScanX=width//tileSize#it is better if image size is a multiple of tilesize
        self.nbScanY=height//tileSize
        self.tileMatrix=[["#"for y in range(self.nbScanY)] for x in range(self.nbScanX)]# a "#" means wall
        self.pointMatrix=[["y"for y in range(self.nbScanY+1)] for x in range(self.nbScanX+1)]# a "y" means accessible point and "n" means inaccessible
        
    def CreateCircuitFromPicture(self):
        self.FillTileMatrix()
        self.ConvertTileMatrixToPointMatrix()
        self.FindStartAndEnd()
        PrintMatrix(self.tileMatrix)
        PrintMatrix(self.pointMatrix)
        self.SaveTileMatrix()
        self.SavePointMatrix()

    def FillTileMatrix(self):
        self.greenPoints=[]
        self.redPoints=[]
        for y in range(self.nbScanY):
            for x in range(self.nbScanX):
                pixelColor=self.picture.getpixel((x*self.tileSize,y*self.tileSize))#image origin in top left corner
                if pixelColor[0]>0 or pixelColor[1]>0 or pixelColor[2]>0:#since the circuit picture is black and white, any channel will do
                    self.tileMatrix[x][y]=" "

                if pixelColor==(0,255,0,255):
                    self.greenPoints.append((x,y))
                elif pixelColor==(255,0,0,255):
                    self.redPoints.append((x,y))

    def SaveTileMatrix(self):
        """this matrix will be used for displaying game only"""
        with open("circuits/"+self.fileName.split(".")[0]+".txt", "w") as textFile:
            print(f"{self.nbScanX};{self.nbScanY}", file=textFile)#save matrix size in first line
            print(f"mapResolution={self.tileSize}", file=textFile)#save the tilesize used to scan to later check if we need to refresh the map to a new resolution 
            for y in range(self.nbScanY):
                line=""
                for x in range(self.nbScanX):
                    line+=self.tileMatrix[x][y]
                print(line, file=textFile)

    def ConvertTileMatrixToPointMatrix(self):
        """we sample 4 values around the scanned pixels a put it in a string in the following order
            _ _
            |1|2|
            |3|4| corresponds to "1234"
            ‾ ‾
            this tuple is they used as a key to read a lookup table and decide if 9 points around pixel are accessible ("y") or not("n")
            _ _ _
            |1|2|3|
            |4|5|6|
            |7|8|9| corresponds to "123456789"
            ‾ ‾ ‾
        """
        tileToPointLUT={
        "    ": "yyyyyyyyy",
        "####": "nnnnnnnnn",
        "#   ": "nnynnyyyy",
        " #  ": "ynnynnyyy",
        "  # ": "yyynnynny",
        "   #": "yyyynnynn",
        "##  ": "nnnnnnyyy",
        " # #": "ynnynnynn",
        "  ##": "yyynnnnnn",
        "# # ": "nnynnynny",
        "### ": "nnnnnnnny",
        "## #": "nnnnnnynn",
        " ###": "ynnnnnnnn",
        "# ##": "nnynnnnnn",
        "#  #": "nnynnnynn",
        " ## ": "ynnnnnnny",
        }#exhaustive
        for x in range(self.nbScanX-1):
            for y in range(self.nbScanY-1):
                key=self.tileMatrix[x][y]+self.tileMatrix[x+1][y]+self.tileMatrix[x][y+1]+self.tileMatrix[x+1][y+1]
                points=tileToPointLUT[key]
                for i in range(9):
                    tempX=i%3#conversion from index to coordinates in 3*3 grid
                    tempY=i//3
                    currentPointState=self.pointMatrix[x+tempX][y+tempY]
                    if currentPointState=="y":#only overwrite if the point is marked accessible
                        self.pointMatrix[x+tempX][y+tempY]=points[i]
    
    def SavePointMatrix(self):
        """this matrix will be used for computing AI behaviour and indicate start and end points"""
        with open("circuits/"+self.fileName.split(".")[0]+"Points.txt", "w") as textFile:
            print(f"{self.nbScanX+1};{self.nbScanY+1}", file=textFile)#save matrix size in first line
            for y in range(self.nbScanY+1):
                line=""
                for x in range(self.nbScanX+1):
                    line+=self.pointMatrix[x][y]
                print(line, file=textFile)

    def FindStartAndEnd(self):
        if len(self.greenPoints)==0:
            print("missing start")
            quit()
        if len(self.redPoints)==0:
            print("missing end")
            quit()
        
        startPosition=AverageVector2(self.greenPoints)#we only have one start point but we can have many finish line points
        self.pointMatrix[startPosition[0]][startPosition[1]]="s"
        for endPosition in self.redPoints:
            self.pointMatrix[endPosition[0]][endPosition[1]]="e"
        

def PrintMatrix(matrix):#helper function accessible from everywhere
        width=len(matrix)
        height=len(matrix[0])
        print("_"*(width+2))
        for y in range(height):
            line="|"
            for x in range(width):
                line+=matrix[x][y]
            print(line+"|")
        print("‾"*(width+2))

def AverageVector2(vector2Array):
    average=[0,0]
    for v in vector2Array:
        average[0]+=v[0]
        average[1]+=v[1]
    average[0]=round(average[0]/len(vector2Array))
    average[1]=round(average[1]/len(vector2Array))
    return average