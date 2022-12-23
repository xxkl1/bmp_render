from main import listXy, colorStrRender
from utils import ensure

def testListXy1():
    l = [1, 2]
    w = 1
    h = 2
    expectOutput = [
        [2],
        [1]
    ]
    r = listXy(l, w, h)
    ensure(r, expectOutput, 'test testListXy1')

def testListXy2():
    l = [3, 4, 1, 2]
    w = 2
    h = 2
    expectOutput = [
        [1, 2],
        [3, 4]
    ]
    r = listXy(l, w, h)
    ensure(r, expectOutput, 'test testListXy2')


# input = [1, 2, 3, 4, 5, 6, 7, 8, 9,]
# w:3 h:3
# ->
# r = [
#     [7, 8, 9],
#     [4, 5, 6],
#     [1, 2, 3],
# ]
def testListXy3():
    l = [1, 2, 3, 4, 5, 6, 7, 8, 9,]
    w = 3
    h = 3
    expectOutput = [
        [7, 8, 9],
        [4, 5, 6],
        [1, 2, 3],
    ]
    r = listXy(l, w, h)
    ensure(r, expectOutput, 'test testListXy3')

# input = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
# w:3 h:4
# ->
# r = [
#     [10, 11, 12],
#     [7, 8, 9],
#     [4, 5, 6],
#     [1, 2, 3],
# ]
def testListXy4():
    l = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,]
    w = 3
    h = 4
    expectOutput = [
        [10, 11, 12],
        [7, 8, 9],
        [4, 5, 6],
        [1, 2, 3],
    ]
    r = listXy(l, w, h)
    ensure(r, expectOutput, 'test testListXy4')

# input = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
# w:4 h:3
# ->
# r = [
#     [9, 10, 11, 12],
#     [5, 6, 7, 8 ],
#     [1, 2, 3, 4],
# ]
def testListXy5():
    l = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,]
    w = 4
    h = 3
    expectOutput = [
        [9, 10, 11, 12],
        [5, 6, 7, 8 ],
        [1, 2, 3, 4],
    ]
    r = listXy(l, w, h)
    ensure(r, expectOutput, 'test testListXy5')

def testListXy():
    testListXy1()
    testListXy2()
    testListXy3()
    testListXy4()
    testListXy5()


def testFixColorStr1():
    input = '01020304'
    output = colorStrRender(input)
    expectOutput = '#030201'
    ensure(output, expectOutput, 'test testFixColorStr1')

def testFixColorStr2():
    input = '010203'
    output = colorStrRender(input)
    expectOutput = '#030201'
    ensure(output, expectOutput, 'test testFixColorStr2')

def testFixColorStr3():
    input = '0102'
    output = colorStrRender(input)
    expectOutput = '#0201'
    ensure(output, expectOutput, 'test testFixColorStr3')

def testFixColorStr4():
    input = '01'
    output = colorStrRender(input)
    expectOutput = '#01'
    ensure(output, expectOutput, 'test testFixColorStr4')

def testFixColorStr():
    testFixColorStr1()
    testFixColorStr2()
    testFixColorStr3()
    testFixColorStr4()

def __main():
    testListXy()
    # 由于全局变量palette暂时运行不了测试
    # testFixColorStr()

if __name__ == "__main__":
    __main()