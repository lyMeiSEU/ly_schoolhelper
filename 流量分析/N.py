import csv
with open('./数据包/01C.csv') as f:
    f_csv=csv.reader(f)
    for row in f_csv:
        print(row,"\n")