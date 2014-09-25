def Binary_Search_as_Recur (query, low, high, a):
    if(low > high):
        return(-1)
    mid = (low + high) / 2
    if(query == a[mid]):
        return(mid)
    if(query < a[mid]):
        return(Binary_Search_as_Recur(query, low, mid - 1, a))
    else:
        return(Binary_Search_as_Recur(query, mid + 1, high, a))

def Binary_Search_as_Iter (query, low, high, a):
    while(low <= high):
        mid = (low + high) / 2
        if(query == a[mid]):
            return(mid)
        if(query < a[mid]):
            high = mid - 1
        else:
            low = mid + 1
    return(-1)
