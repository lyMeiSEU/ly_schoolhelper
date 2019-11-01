import xlrd
import csv
import matplotlib.pyplot as plt
from collections import defaultdict

table=xlrd.open_workbook("rider_beacon_and_gps_20181001.xlsx")
sheet=table.sheets()[0]
rows=sheet.nrows
rider=defaultdict(list)
rider_lo=defaultdict(list)
rider_la=defaultdict(list)
rider_time=defaultdict(list)
longitude=[]
latitude=[]

for row in range(1,rows):
    rowvalue=sheet.row_values(row)
    Lo_gps=rowvalue[9]
    La_gps=rowvalue[10]
    longitude.append(Lo_gps)
    latitude.append(La_gps)
    time_gps=rowvalue[5]
    val=[Lo_gps,La_gps,time_gps]
    rider[str(rowvalue[0])].append(val)
    rider_lo[str(rowvalue[0])].append(Lo_gps)
    rider_la[str(rowvalue[0])].append(La_gps)
    rider_time[str(rowvalue[0])].append(time_gps)

i=0
for key_rider in rider.keys():
    print(rider_lo[key_rider])
    csvF=open('loc&gps'+str(key_rider)+'.csv','w+',newline="")
    csvwriter=csv.writer(csvF)
    csvwriter.writerow(['longitude','latitude'])
    csvwriter.writerow(rider_lo[key_rider])
    csvwriter.writerow(rider_la[key_rider])
    csvwriter.writerow(rider_time[key_rider])
    csvF.close()
    plt.scatter(rider_lo[key_rider],rider_la[key_rider])
    if i==0:
        plt.plot(rider_lo[key_rider],rider_la[key_rider],'r')
    elif i==1:
        plt.plot(rider_lo[key_rider],rider_la[key_rider],'g')
    elif i==2:
        plt.plot(rider_lo[key_rider],rider_la[key_rider],'b')
    i+=1
plt.show()
plt.close()

    
