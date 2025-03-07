from PIL import Image


CIRCUIT="testCircuit.png"
TILE_SIZE=16#how many pixel per tile #e.g. 512*512px picture with tileize 16 will result in 32*32 tiles map

picture=Image.open(CIRCUIT,"r")
width,height=picture.size
nbScanX=width//TILE_SIZE#it is better if image size i a multiple of tilesize
nbScanY=height//TILE_SIZE
tileMatrix=[["#"for y in range(nbScanY)] for x in range(nbScanX)]# a "#" means wall
pointMatrix=[["y"for y in range(nbScanY+1)] for x in range(nbScanX+1)]# a "y" means accessible point and "n" means inaccessible

def FillTileMatrix():
    for y in range(nbScanY):
        for x in range(nbScanX):
            pixelColor=picture.getpixel((x*TILE_SIZE,y*TILE_SIZE))#image origin in top left corner
            if pixelColor[0]>0:#since the circuit picture is black and white, any channel will do
                tileMatrix[x][y]=" "

def PrintMatrix(matrix):
    width=len(matrix)
    height=len(matrix[0])
    print("_"*(width+2))
    for y in range(height):
        line="|"
        for x in range(width):
            line+=matrix[x][y]
        print(line+"|")
    print("‾"*(width+2))

def SaveTileMatrix():
    """this matrix will be used for displaying game only"""
    with open(CIRCUIT.split(".")[0]+".txt", "w") as textFile:
        print(f"{nbScanX};{nbScanY}", file=textFile)#save matrix size in first line
        for y in range(nbScanY):
            line=""
            for x in range(nbScanX):
                line+=tileMatrix[x][y]
            print(line, file=textFile)

def ConvertTileMatrixToPointMatrix():
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
    for x in range(nbScanX-1):
        for y in range(nbScanY-1):
            key=tileMatrix[x][y]+tileMatrix[x+1][y]+tileMatrix[x][y+1]+tileMatrix[x+1][y+1]
            points=tileToPointLUT[key]
            for i in range(9):
                tempX=i%3#conversion from index to coordinates in 3*3 grid
                tempY=i//3
                currentPointState=pointMatrix[x+tempX][y+tempY]
                if currentPointState=="y":#only overwrite if the point is marked accessible
                    pointMatrix[x+tempX][y+tempY]=points[i]

    
def SavePointMatrix():
    """this matrix will be used for computing AI behaviour"""
    with open(CIRCUIT.split(".")[0]+"Points.txt", "w") as textFile:
        print(f"{nbScanX+1};{nbScanY+1}", file=textFile)#save matrix size in first line
        for y in range(nbScanY+1):
            line=""
            for x in range(nbScanX+1):
                line+=pointMatrix[x][y]
            print(line, file=textFile)

"""FillTileMatrix()
ConvertTileMatrixToPointMatrix()
PrintMatrix(tileMatrix)
PrintMatrix(pointMatrix)
SaveTileMatrix()
SavePointMatrix()"""
