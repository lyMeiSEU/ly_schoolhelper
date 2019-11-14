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
    timeshac=xlrd.xldate_as_tuple(rowvalue[8],0)
    time_shopac=datetime.datetime(timeshac[0],timeshac[1],timeshac[2],timeshac[3],timeshac[4],timeshac[5])
    timear=xlrd.xldate_as_tuple(rowvalue[10],0)
    time_rdarrive=datetime.datetime(timear[0],timear[1],timear[2],timear[3],timear[4],timear[5])
    time_delta=time_rdarrive-time_shopac
                            #  骑手ID       商家接单     骑手离开          差值
    ord_shLoLa_urLoLa_alltime=[rowvalue[1],time_shopac,time_rdarrive,time_delta, [rowvalue[13]]]
    order_value[str(rowvalue[2])].append(ord_shLoLa_urLoLa_alltime)
    
csvF=open('sku.csv','w+',newline="")
csvwriter=csv.writer(csvF)
for key in order_value.keys():
    csvwriter.writerow([key,order_value[key],order_value[key][0][3].seconds,len(order_value[key][0][4])])

print(order_value)