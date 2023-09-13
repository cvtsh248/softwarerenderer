import random

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

    # maxim = 0
    # minim = 1000000
    # pivot = 0
    # temp = 0
    # for piv in (arr):
    #     if piv > maxim:
    #         maxim = piv
    # for piv in (arr):
    #     if piv < minim:
    #         minim = piv
    # for c, tem in enumerate(arr):
    #     if (tem < maxim and tem > minim):
    #         pivot = tem
    #         temp = c
    #         break
    # print(maxim, minim)
    # arr[temp] = arr[-1]

# def partition(arr):
#     maxim = 0
#     minim = 1000000
#     for itr in (arr):
#         if itr > maxim:
#             maxim = itr
#     for itr in (arr):
#         if itr < minim:
#             minim = itr

#     ct = 0
#     while arr[-1] == maxim:
#         tmp = maxim
#         arr[-1] = arr[ct]
#         arr[ct] = tmp
#         ct+=1
    
#     while arr[-1] == minim:
#         tmp = minim
#         arr[-1] = arr[ct]
#         arr[ct] = tmp
#         ct+=1
#     pivot = arr[-1]
#     print(pivot)
#     i_index = -1
#     j_index = 0
#     print(arr)
#     while (j_index < len(arr) and i_index < len(arr)-1):
#         if (arr[j_index] > pivot):
#             j_index += 1
#         elif (arr[j_index] <= pivot):
#             i_index += 1
#             temp = arr[i_index]
#             arr[i_index] = arr[j_index]
#             arr[j_index] = temp

#     return i_index

# def partition(arr, low, high):
#     pivot = arr[high]
#     #print(pivot)
#     i_index = -1+low
#     j_index = 0+low
#     #print(arr)
#     while (j_index < high+1 and i_index < high-1):
#         if (arr[j_index] > pivot):
#             j_index += 1
#         elif (arr[j_index] <= pivot):
#             i_index += 1
#             temp = arr[i_index]
#             arr[i_index] = arr[j_index]
#             arr[j_index] = temp
#     #print(arr)
#     return i_index

# def quicksort(arr, low, high):
#     maxim = 0
#     minim = 1000000
#     print(high-low)
#     #print(arr)
#     if high-low > 0:
#         for itr in (arr):
#             if itr > maxim:
#                 maxim = itr
#         for itr in (arr):
#             if itr < minim:
#                 minim = itr

#         ct = 0
#         while arr[-1] == maxim:
#             tmp = maxim
#             arr[-1] = arr[ct]
#             arr[ct] = tmp
#             ct+=1
        
#         while arr[-1] == minim:
#             tmp = minim
#             arr[-1] = arr[ct]
#             arr[ct] = tmp
#             ct+=1

#         p = partition(arr,0,len(arr)-1)
#         arr = quicksort(arr, low, p-1)
#         arr = quicksort(arr, p+1, high)


#print(lerpBilinear([5,19], [12,9]))
#print(quicksort([1,2,9,7,3,4]))
#print(partition([110,42,10,2,43,102,1,1500]))

def partition(arr, low, high):
    pivot = arr[random.randint(low, high)]
    if (j < pivot)



a = [0,3,4,51,4,32,9,21,12,3,7]
(quicksort(a,0,len(a)))