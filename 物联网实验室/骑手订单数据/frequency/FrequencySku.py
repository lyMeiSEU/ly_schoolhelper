
import xlrd
import csv
import os
import random
import datetime
import math
import matplotlib.pyplot as plt
from collections import defaultdict

file_order = "../data_for_luoyu_tracking_detail_with_sku_201810"

table=xlrd.open_workbook(file_order+'.xlsx')
sheet = table.sheets()[0]
rows = sheet.nrows
order_value=defaultdict(list)

for row in range(1, rows):
    rowvalue = sheet.row_values(row)
    if rowvalue[2] in order_value.keys():
        order_value[rowvalue[2]][0][4].append(rowvalue[13])
        continue
    timear=xlrd.xldate_as_tuple(rowvalue[10],0)
    time_rdarrive=datetime.datetime(timear[0],timear[1],timear[2],timear[3],timear[4],timear[5])
    if rowvalue[13] in order_value.keys():
        order_value[rowvalue[13]][len(order_value[rowvalue[13]])-1]=time_rdarrive-order_value[rowvalue[13]][len(order_value[rowvalue[13]])-1]
        order_value[rowvalue[13]].append(time_rdarrive)
    else:
        order_value[rowvalue[13]].append(time_rdarrive)
                            

    
# csvF=open('skuFrequency.csv','w+',newline="")
# csvwriter=csv.writer(csvF)
# for key in order_value.keys():
#     csvwriter.writerow(order_value[key])

i=0
for key in order_value.keys():
    i=i+1
    if i==3 or i==6 or i==72 or i==153 or i==170 or i==196 or i==217:
        print(key)
print(order_value)