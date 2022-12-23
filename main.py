import tkinter as tk
import math
from utils import *

# 调色板，fixColorStr有用到，得放全局
palette = None

# data btyes类型
def toNumber(data):
    length = len(data)
    num = 0
    for i in range(length):
        # bmp中，多字节转10进制，低位在左边，高位在右边
        # 一个字节之间是是进256
        num += int(data[i] * math.pow(256, i))
    return num

# data btyes类型
# 返回 bytearray类型
def contentBMP(data):
    # 内容区的开始
    start = toNumber(data[0x0a:0x0a+4])
    print('content start:', hex(start))
    # 内容区为开始偏移量一直到结尾
    c = data[start:]
    return c

# data btyes类型
# 返回 number类型
# 实际bmp字节表现的色深，由内容区长度和像素数得到
def deepBMP(content, width, height):
    lenContent = len(content)
    countPixels = width * height
    print('countPixels:', countPixels)
    bitsOneByte = 8
    r = int(lenContent / countPixels * bitsOneByte)
    return r

# data btyes类型
# 返回 number类型
# deepFormatBMP是bmp表示的深度，在16bit深度图片解析会用到，直接从bmp信息得到
def deepFormatBMP(data):
    deep = toNumber(data[0x1c:0x1c+2])
    return deep

# data btyes类型
# 返回 number类型
def widthBMP(data):
    w = toNumber(data[0x12:0x16])
    return w

# data btyes类型
# 返回 number类型
def heightBMP(data):
    h = toNumber(data[0x16:0x1A])
    return h

# l array类型 一维数组
# width number类型 宽度
# heigth number类型 高度
# 返回 array类型，表达xy轴的二维数组

# 注意，由于bmp的设计问题，转二维数组的时候，需要从下到上，从左到右，具体看下面例子
# input = [3, 4, 1, 2,]
# ->
# r = [
#     [1, 2],
#     [3, 4],
# ]
# 坐标1是y，坐标2是x
# r[0][0] = 1, r[0][1] = 2, r[1][1] = 4, r[1][0] = 3
def listXy(l, width, heigth):
    # 根据width, heigth弄一个空白的二维数组
    r = emptyListXy(width, heigth)
    # 一维数组按照bmp顺序填充，变成正常的渲染二维数组
    # 先从最下面开始，从左到右边填充好
    iL = 0
    iRY = len(r) - 1
    iRX = 0
    while True:
        r[iRY][iRX] = l[iL]
        nextIX = iRX + 1
        # 如果x超出下标，更新nextIY，nextIX重置
        if nextIX > width - 1:
            nextIY = iRY - 1
            nextIX = 0
            # 直接赋值不然会有referenced before assignment
            iRY = nextIY
            # 如果y超出下标，填充完毕
            if nextIY < 0:
                break
        # 根据上面的next x，更新下次的x y坐标
        iRX = nextIX
        # 下次iL要加1
        iL = iL + 1
    return r

# color string 直接解析bmp得到的颜色字符串，不带#前缀的
# return string tkinter能渲染的颜色字符串
# 32深度: '#01020304' -> '#030201' tkinter无法渲染透明度，需要把透明度去掉
# 24深度: '#010203' -> '#030201'
# 16深度: '#0102' -> '#0102' 这个还没出现过，遇到再处理，先直接返回
# 8深度: '#01' -> '#01'
def colorStrRender(color):
    global palette
    lenStr = len(color)
    l = chunkList(color, 2)
    # lenStr长度是color - 1，去掉#，'#01020304'的长度是9，lenStr是8
    # 32深度/24深度
    # 8长度的颜色字符串tk无法显示，所以8长度的要去掉透明度
    if lenStr == 8 or lenStr == 6:
        return '#'+ l[2] + l[1] + l[0]
    # 16深度
    elif lenStr == 4:
        return '#'+ l[1] + l[0]
    # 8深度
    elif lenStr == 2:
        # lenStr2是8深度，一个字节代表一个像素，一个字节代表调色板的下标
        index = int(color, 16)
        return palette[index]
    # 4深度
    elif lenStr == 1:
        # lenStr1是4深度，半个字节代表调色板的下标
        index = int(color, 16)
        return palette[index]
    else:
        return color


# 将bmp内容区转换成渲染树
# \x03\x03\x03\x03\x04\x04\x04\x04\x01\x01\x01\x01\x02\x02\x02\x02'
# ->
# [\x03\x03\x03\x03, \x04\x04\x04\x04, \x01\x01\x01\x01, \x02\x02\x02\x02]
# ->
# ['03030303', '04040404' '01010101', '02020202']
# ->
# ['#03030303', '#04040404' '#01010101', '#02020202']
# ->
# [
#   ['#01010101', '#02020202'],
#   ['#03030303', '#04040404']
# ]
# 参数 data bmp 内容区
# 参数 deep bmp 深度
def renderList(data, deep, width, height):
    # 一维数组根据deep切割成块
    if deep >= 8:
        lenChunk = int((deep / 8))
        r = chunkList(bytearray(data), lenChunk)
        # 转字符串
        for i in range(len(r)):
            r[i] = r[i].hex()
            r[i] = colorStrRender(r[i])
        # 转为xy布局二维数组
        r = listXy(r, width, height)
        return r
    # deep 4 深度需要单独处理，需要弄成元素为半个字节的数组
    # 这样处理后，长度会变成两倍，因为一个字节拆开成两半
    elif deep == 4:
        l = bytearray(data)
        lRender = []
        for i in range(len(l)):
            cur = l[i]
            # bytes转为0x00字符串
            cur = hex(cur)
            # 取消0x
            cur = cur.replace('0x', '')
            # 可能有0x1这种情况，需要加上0前缀
            if len(cur) < 2:
                cur = '0' + cur
            first = cur[0]
            two = cur[1]
            # 根据调色板获得真正的颜色值
            first = colorStrRender(first)
            two = colorStrRender(two)
            lRender.append(first)
            lRender.append(two)
        r = listXy(lRender, width, height)
        return r

# list array renderList得到的xy渲染数组
# 例如 [
#   ['#01010101', '#02020202'],
#   ['#03030303', '#04040404']
# ]
# 该函数仅渲染图片，不参与bmp解析
def renderByXyList(list, width, height):
    window = tk.Tk()
    # canvas有默认边框2px，所以上下左右要加上4px
    canvasWidth = width+4
    canvasHeight = height+4
    # canvas画居中图片的套路写法
    canvas = tk.Canvas(window, width=canvasWidth, height=canvasHeight, bg="#fff")
    canvas.pack()
    img = tk.PhotoImage(width=canvasHeight, height=canvasHeight)
    canvas.create_image((canvasHeight//2, canvasHeight//2), image=img, state="normal")

    for y in range(height):
        for x in range(width):
            color = list[y][x]
            # canvas有边框2px，画的时候，x，y需要偏移3px
            img.put(color, (x + 3, y + 3))

    tk.mainloop()


def lenPaletteBMP(data, deep):
    # 2e是代表调色板的颜色数
    count = toNumber(data[0x2e:0x2e+4])
    # 如果为0，那么默认为2的deep次方
    if count == 0:
        count =  int(math.pow(2, deep))
    print('count palette:', count)
    return count

# data btyes类型
# 返回 array 调色板 ['#010101', '#020202', '#030303', ...]， 下标表示内容区颜色
def paletteBMp(data, sizeDIB, deep):
    # 调色板一个色为4个字节
    lenBytesOneColor = 4
    lenPalette = lenPaletteBMP(data, deep)
    # 一般来说调色板的开始是DIB头大结束，0x0e是DIB头的开始
    start = 0x0e + sizeDIB
    end = start+lenPalette * lenBytesOneColor
    l = bytearray(data[start:end])
    l = chunkList(l, lenBytesOneColor)
    r = []
    for i in range(len(l)):
        cur = l[i]
        cur = cur.hex()
        cur = colorStrRender(cur)
        r.append(cur)
    return r

# 压缩方式
def compressionBMP(data):
    return toNumber(data[0x1e:0x1e+4])

# DIB头大小，根据DIB头大小可以判断是用那种格式的DIB头
def sizeDIBBMp(data):
    return toNumber(data[0x0e:0x0e+4])

def endDIBBMp(data):
    size = sizeDIBBMp(data)
    return hex(0x0e + size - 1)

# 如果deep<=16 >= 8，deepFormat不等于deep，content里面有多余的00，需要去除
# 根据计算出理论上deepFormat的字节数，用当前content-这个理论的字节数，得出差的count数
# 例如, 01 01 00 01 01 00，理论是4个字节，实际是6个字节，差两个，这两个是补0，字节是00，位置是实际字节数组chunk chunklen是相差字节数
# chunk的结果的末尾就是补0的位置，这里是两个01后面的00

# 理论上是这样，但是对于deep 16，deepFormat 4，16bit_color_small.bmp不适用，相差6，但是分割数组是2。。不理了，奇奇怪怪

# content btyes类型
# deep number
# deepFormat number
def fixContent(content, deep, deepFormat, width, height):
    if deep > 8 and deep <= 16 and deep != deepFormat:
        pixelsCount = width * height
        lenBytesOnePixel = deepFormat / 8
        # 得到理论的字节数
        lenBytesNeed = int(lenBytesOnePixel * pixelsCount)
        lenC = len(content)
        # 得到理论字节数和实际字节数相差的值
        lenDiff = len(content) - lenBytesNeed
        # 需要把数组分成两半去删除多余的0的数量，面向测试图片编程
        lenDiff = lenDiff / 2
        # 把数组变成两半
        content = chunkList(bytearray(content), int(lenC / 2))
        l = []
        # 去掉多余的00，并合并回一个数组
        for d in content:
            d = d[0:(int(len(d) - lenDiff))]
            l.extend(d)
        # 数组转回bytes返回
        content = bytes(l)
        return content
    else:
        return content

# 得到真正渲染用的色深
def deepRender(deep, deepFormat):
    if deep > 8 and deep <= 16:
        return deepFormat
    else:
        return deep

# data btyes类型
# 返回 number类型
def render(data):
    c = contentBMP(data)
    compression = compressionBMP(data)
    print('compression:', compression)
    w = widthBMP(data)
    print('w:', w)
    h = heightBMP(data)
    print('h:', h)
    deep = deepBMP(c, w, h)
    print('deep:', deep)
    deepFormat = deepFormatBMP(data)
    print('deepFormat:', deepFormat)
    c = fixContent(c, deep, deepFormat, w, h)
    sizeDIB = sizeDIBBMp(data)
    print('sizeDIB:', sizeDIB)
    # 124是常见的DIB头，其他是非常见
    if sizeDIB != 124:
        print('非常见DIB')
    print('end DIB:', endDIBBMp(data))
    dR = deepRender(deep, deepFormat)
    global palette
    palette = paletteBMp(data, sizeDIB, dR)
    l = renderList(c, dR, w, h)
    renderByXyList(l, w, h)

def __main():
    # 读取bmp文件二进制
    d = readFile('./image_test/16bit_color_small.bmp')
    # # 根据bmp格式进行图片渲染
    render(d)

if __name__ == "__main__":
    __main()