import xlrd
import csv
import os
import random
import datetime
import math
import matplotlib.pyplot as plt
from collections import defaultdict

file_order = "data_for_zeyu_3"
file_sku="data_for_luoyu_order_sku_20181001"

#print(rider)

table1=xlrd.open_workbook(file_sku+'.xlsx')
sheet1 = table1.sheets()[0]
rows1 = sheet1.nrows
order_sku=defaultdict(list)

for row in range(1, rows1):
    rowvalue = sheet1.row_values(row)
    order_sku
    order_sku[str(rowvalue[1])].append(rowvalue[2])
    
table2=xlrd.open_workbook(file_order+'.xlsx')
sheet2 = table2.sheets()[0]
rows2 = sheet2.nrows
rider_time_value=defaultdict(list)

for row in range(1, rows2):
    rowvalue = sheet2.row_values(row)
    timeshac=xlrd.xldate_as_tuple(rowvalue[8],0)
    time_shopac=datetime.datetime(timeshac[0],timeshac[1],timeshac[2],timeshac[3],timeshac[4],timeshac[5])
    timear=xlrd.xldate_as_tuple(rowvalue[10],0)
    time_rdarrive=datetime.datetime(timear[0],timear[1],timear[2],timear[3],timear[4],timear[5])
    time_delta=time_rdarrive-time_shopac
                            #  骑手ID       商家接单     骑手离开          差值
    ord_shLoLa_urLoLa_alltime=[rowvalue[1],time_shopac,time_rdarrive,time_delta, order_sku[rowvalue[2]]]
    rider_time_value[str(rowvalue[2])].append(ord_shLoLa_urLoLa_alltime)
    
csvF=open('shop.csv','w+',newline="")
csvwriter=csv.writer(csvF)
for key in rider_time_value.keys():
    csvwriter.writerow([key,rider_time_value[key],rider_time_value[key][0][3].seconds,len(rider_time_value[key][0][4])])

print(rider_time_value)