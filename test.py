def lerpInt(a, b):
    buffer = []
    for i in range(0, b-a+1):
        buffer.append(a+i)
    return buffer

def lerpBilinear(a, b):
    buffer = []
    x = a[0]
    gradient = (b[1]-a[1])/(b[0]-a[0])
    constant = a[1]-a[0]*gradient

    if (b[0]-a[0]) > 0:
        while x != b[0]+1:
            y = gradient*x + constant
            buffer.append([x,y])
            x+=1
    elif (b[0]-a[0]) < 0:
        while x != b[0]+1:
            y = gradient*x + constant
            buffer.append([x,y])
            x-=1
    return(buffer)

print(lerpBilinear([5,19], [12,9]))