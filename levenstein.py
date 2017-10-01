import win32api

def levensteinRecursive3(stringOriginal, stringResult):
    if stringOriginal == "":
        return len(stringResult)
    if stringResult == "":
        return len(stringOriginal)

    if stringResult[0] != stringOriginal[0]:
        firstNotSame = 1
    else:
        firstNotSame = 0

    return min(levensteinRecursive3(stringOriginal[1:], stringResult) + 1,
               levensteinRecursive3(stringOriginal, stringResult[1:]) + 1,
               levensteinRecursive3(stringOriginal[1:], stringResult[1:]) +
               firstNotSame)

def levensteinIterative3(stringOriginal, stringResult):
    if stringOriginal == "":
        return len(stringResult)
    if stringResult == "":
        return len(stringOriginal)

    columns = len(stringResult) + 1
    rows = len(stringOriginal) + 1
    
    storage = [[i for i in range(columns)], [0] * columns]
            
    for i in range(1, rows):
        storage[1][0] = storage[0][0] + 1
        for j in range(1, columns):

            if stringResult[j - 1] != stringOriginal[i - 1]:
                currentNotSame = 1
            else:
                currentNotSame = 0

            storage[1][j] = min(storage[0][j] + 1, storage[1][j - 1] + 1,
                                storage[0][j - 1] + currentNotSame)
        storage[0], storage[1] = storage[1], storage[0]

    return storage[0][-1]

def levensteinIterative4(stringOriginal, stringResult):
    columns = len(stringResult) + 1
    rows = len(stringOriginal) + 1
    
    if rows < 3 or columns < 3:
        return levesteinIterative3(stringOriginal, stringResult)

    storage = [[i for i in range(columns)], [0] * columns, [0] * columns]
    storage[1][0] = storage[0][0] + 1
    for j in range(1, columns):
        if stringResult[j - 1] != stringOriginal[0]:
            currentNotSame = 1
        else:
            currentNotSame = 0

        storage[1][j] = min(storage[0][j] + 1, storage[1][j - 1] + 1,
                            storage[0][j - 1] + currentNotSame)
    for i in range(2, rows):
        storage[2][0] = storage[1][0] + 1
        if stringResult[0] != stringOriginal[i - 1]:
            currentNotSame = 1
        else:
            currentNotSame = 0
                
        storage[2][1] = min(storage[1][1] + 1, storage[2][0] + 1,
                             storage[1][0] + currentNotSame)
         
        for j in range(2, columns):
            if stringResult[j - 1] != stringOriginal[i - 1]:
                currentNotSame = 1
            else:
                currentNotSame = 0
                
            storage[2][j] = min(storage[1][j] + 1, storage[2][j - 1] + 1,
                                storage[1][j - 1] + currentNotSame,
                                storage[0][j - 2] + 1)
             
        storage[0], storage[1], storage[2] = storage[1], storage[2], storage[0]
        
    return storage[1][-1]
    

str1 = "manhattan"
str2 = "manahttan"

tick1 = win32api.GetTickCount()
#levRec3 = levensteinRecursive3(str1, str2)
levRec3Time = win32api.GetTickCount() - tick1

tick1 = win32api.GetTickCount()
levIt3 = levensteinIterative3(str1, str2)
levIt3Time = win32api.GetTickCount() - tick1

tick1 = win32api.GetTickCount()
levIt4 = levensteinIterative4(str1, str2)
levIt4Time = win32api.GetTickCount() - tick1

#print(levRec3, " ", levRec3Time)
print(levIt3, " ", levIt3Time)
print(levIt4, " ", levIt4Time)
