
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
shop_Order=defaultdict(list)


for row in range(1, rows):
    rowvalue = sheet.row_values(row)
    timear=xlrd.xldate_as_tuple(rowvalue[10],0)
    time_rdarrive=datetime.datetime(timear[0],timear[1],timear[2],timear[3],timear[4],timear[5])
    if rowvalue[3] in shop_Order.keys():
       shop_Order[rowvalue[3]][time_rdarrive.hour][int(time_rdarrive.minute/10)]=shop_Order[rowvalue[3]][time_rdarrive.hour][int(time_rdarrive.minute/10)]+1
    else:
        for i in range(0,24):
            shop_Order[rowvalue[3]].append([])
            for j in range(0,6):
                shop_Order[rowvalue[3]][i].append(0)
        shop_Order[rowvalue[3]][time_rdarrive.hour][int(time_rdarrive.minute/10)]=1


            
# csvF=open('skuFrequency.csv','w+',newline="")
# csvwriter=csv.writer(csvF)
# for key in order_value.keys():
#     csvwriter.writerow(order_value[key])

csvF=open('OrderFrequency.csv','w+',newline="")
csvwriter=csv.writer(csvF)
for key in shop_Order.keys():
    for item in shop_Order[key]:
        csvwriter.writerow(item)