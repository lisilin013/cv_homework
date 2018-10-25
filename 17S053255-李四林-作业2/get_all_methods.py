# _*_ coding: utf-8 _*_

# using dynamic programming 
def GetAllMethods(n):
    if n<1:
        print("input n must >= 1!")
        return 0
            
    res = []    
    for i in xrange(0,10):
        res.append(2**i)
        # print(res[x])

    # 1-10
    if n <= 10:
        return res[n-1]

    pos = 0
    result = 0
    while n-10>0: #>=11
        pos %= 10;
        result = sum(res) % 10677;
        res[pos] = result;
        pos +=1;
        n -=1;#update
    return result


if __name__ == '__main__':
    for i in xrange(1,100000001):
        if i==10 or i==100 or i==1000or i==10000or i==100000 or i==1000000 or i==10000000 or i==100000000 :
            res =  GetAllMethods(i)
            print('GetAllMethods(%d): %d' % (i,res))


