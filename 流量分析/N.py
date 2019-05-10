import csv
with open('./数据包/01C.csv') as f:
    f_csv=csv.reader(f)
    dict={'TCPNo':'SIZE'}
    var=""
    for row in f_csv:
        var1=row
        var=var+var1
    print(var)

