def ensure(output, expectOutput, message):
    if output == expectOutput:
        print(message + ' test ok')
    else:
        print(message + ' test fail')
        print('expect:', expectOutput)
        print('but:', output)

# [1, 2, 3, 4] lenChunk: 1 -> [[1], [2], [3], [4]]
def chunkList(list, lenChunk):
    r = []
    for i in range(len(list)):
        if i % lenChunk == 0:
            r.append(list[i:i+lenChunk])
    return r

def readFile(path):
    f = open(path, 'rb')
    b = f.read()
    return b

def emptyListXy(w, h):
    l = []
    for i in range(h):
        l.append([])

    for iX in range(len(l)):
        for i in range(w):
            l[iX].append(None)
    return l