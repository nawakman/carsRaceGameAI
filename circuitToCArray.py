from pictureToCircuit import *

class circuitToCArray:
    def __init__(self,fileName,tileSize):
        pToC = pictureToCircuit(fileName,tileSize)
        pToC.CreateCircuitFromPicture()
        self.printCCircuit(pToC)


    def printCCircuit(self,pToC):
        # transposee de la matrice, pour lettre en format matrice[i][j]
        matrix = list(map(list, zip(*pToC.pointMatrix)))
        print(f"char circuit[{len(matrix[0])}][{len(matrix)}] = {{ ",end='')
        for i, element in enumerate(matrix):
            if (i==len(matrix)-1):
                print("{'"+"','".join(element)+"'} };")
            else :
                print("{'"+"','".join(element)+"'},",end='')
        
        

circuitToCArray("test1.png",10)