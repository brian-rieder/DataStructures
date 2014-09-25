from math import log10, pow, factorial, sqrt
 
trestraintSec = pow(10,6)
trestraintMin = trestraintSec * 60
trestraintHr = trestraintMin * 60
trestraintDay = trestraintHr * 24
trestraintMon = trestraintDay * 31
trestraintYr = trestraintDay * 365
trestraintCen = trestraintYr * 100

# nsec = log10(trestraintSec) / log10(2)
# nmin = log10(trestraintMin) / log10(2)
# nhr = log10(trestraintHr) / log10(2)
# nday = log10(trestraintDay) / log10(2)
# nmon = log10(trestraintMon) / log10(2)
# nyr = log10(trestraintYr) / log10(2)
# ncen = log10(trestraintCen) / log10(2)

# print("OUTPUT:")
# print("Times run in a second: ", nsec) #'%.2e' % nsec)
# print("Times run in a minute: ", nmin) #'%.2e' %  nmin)
# print("Times run in an hour:  ", nhr) #'%.2e' %  nhr)
# print("Times run in a day:    ", nday) #'%.2e' %  nmon)
# print("Times run in a month:  ", nmon) #'%.2e' %  nday)
# print("Times run in a year:   ", nyr) #'%.2e' %  nyr)
# print("Times run in a century:", ncen) #'%.2e' %  ncen)
# print("\n\n")

n = 68610956693000
trun = 0

while trun < trestraintCen:
    print("Time required to run:",trun)
    # print("Number of executions:", n)
    print("Number of executions:", '%.2e' %n)
    

    n += 1
    


    #Function 1: log2(n)
    #trun = log10(n)/log10(2)
     
    #Function 2: sqrt(n)
    #trun = sqrt(n)
 
    #Function 3: n
    #trun = n
 
    #Function 4: nlog2(n)
    trun = n * (log10(n)/log10(2))
 
    #FINISHED...
    #Function 5: n^2
    #trun = pow(n,2)
 
    #FINISHED...
    #Function 6: n^3
    #trun = pow(n,3)
 
    #FINISHED...
    #Function 7: 2^n
    #trun = pow(2,n)
 
    #FINISHED...
    #Function 8: n!
    #trun = factorial(n)

print("\n\n")
    
