
import xlrd
import csv
import os
import random
import datetime
import math
import matplotlib.pyplot as plt
from collections import defaultdict


#python计算两点间距离-m

def geodistance(lng1,lat1,lng2,lat2):
    lng1, lat1, lng2, lat2 = map(math.radians, [lng1, lat1, lng2, lat2])
    dlon=lng2-lng1
    dlat=lat2-lat1
    a=math.sin(dlat/2)**2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon/2)**2 
    dis=2*math.asin(math.sqrt(a))*6371*1000
    return dis


file_rider = "rider_beacon_and_gps_20181001"
file_order = "data_for_zeyu_3"
table1 = xlrd.open_workbook(file_rider+'.xlsx')
sheet1 = table1.sheets()[0]
rows1 = sheet1.nrows
rider = defaultdict(list)

for row in range(1, rows1):
    rowvalue = sheet1.row_values(row)
    time=datetime.datetime.strptime(rowvalue[5],"%Y-%m-%d %H:%M:%S.0")
    LoLaTm_val=[rowvalue[9],rowvalue[10],time]
    if LoLaTm_val in rider[str(rowvalue[0])]:
        continue
    rider[str(rowvalue[0])].append(LoLaTm_val)

#print(rider)

e_c=0.01
table2=xlrd.open_workbook(file_order+'.xlsx')
sheet2 = table2.sheets()[0]
rows2 = sheet2.nrows
rider_time_value=defaultdict(list)
val_timestep=defaultdict(list)
val_desten=defaultdict(list)

for row in range(1, rows2):
    rowvalue = sheet2.row_values(row)
    timesh=xlrd.xldate_as_tuple(rowvalue[10],0)
    time_shop=datetime.datetime(timesh[0],timesh[1],timesh[2],timesh[3],timesh[4],timesh[5])
    timear=xlrd.xldate_as_tuple(rowvalue[12],0)
    time_arrive=datetime.datetime(timear[0],timear[1],timear[2],timear[3],timear[4],timear[5])
                            #  订单号    到店时间     送达时间
    ord_shLoLa_urLoLa_alltime=[rowvalue[2],time_shop,time_arrive]
    rider_time_value[str(rowvalue[1])].append(ord_shLoLa_urLoLa_alltime)
    flag= False
    val_desten[rowvalue[2]]=0
    last_timesep=rider[str(rowvalue[1])][0]
    for timestep in rider[str(rowvalue[1])]:
        # print(timestep[2]-time_shop)
        # print(timestep[2]-time_arrive)
        if (timestep[2]>time_shop and timestep[2]-time_shop<datetime.timedelta(minutes=5)) or \
        (timestep[2]<time_shop and time_shop-timestep[2]<datetime.timedelta(minutes=5)):
            flag=True
        if (timestep[2]>time_arrive and timestep[2]-time_arrive<datetime.timedelta(minutes=5)) or \
        (timestep[2]<time_arrive and time_arrive-timestep[2]<datetime.timedelta(minutes=5)):
            flag=False
            break
        val_timestep[rowvalue[2]].append(timestep)
        if last_timesep!=[]:
            val_desten[rowvalue[2]]=val_desten[rowvalue[2]]+geodistance(last_timesep[0],last_timesep[1],timestep[0],timestep[1])
        last_timesep=timestep
csvF=open('dest.csv','w+',newline="")
csvwriter=csv.writer(csvF)
for key in val_timestep.keys():
    csvwriter.writerow([key,val_desten[key],val_timestep[key]])

print(rider_time_value)