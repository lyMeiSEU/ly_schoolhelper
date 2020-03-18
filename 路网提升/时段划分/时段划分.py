

# -*- coding: utf-8 -*-


import pandas as pd
from interval import Interval
# from datetime import datetime
import datetime

import numpy as np
import copy
from os import listdir,rename,makedirs
from os.path import  isfile,join,exists
print("请输入n的值······")
n=int(input())
N=int(n)-1

while True:
    print("请输入X的值······")

    X=int(input())
    if not isinstance(X,int) or X<=1:
        print("输入不合法，请重新输入")
    else:
        break
filePath="E://crossing//check//csv"
savePath="E://crossing//check//result"

directoryFiles=[f for f in listdir(filePath) ]

def gen_time(time_list):
    r = []
    for b in time_list:
        h = int(b.split(":")[0])
        m = int(b.split(":")[1])
        if m >= 0 and m < 5:
            time = str(h) + ":00:00"
        if (m >= 5 and m < 15):
            time = str(h) + ":10:00"
        if m >= 15 and m < 25:
            time = str(h) + ":20:00"
        if (m >= 25 and m < 35):
            time = str(h) + ":30:00"
        if m >= 35 and m < 45:
            time = str(h) + ":40:00"
        if (m >= 45 and m < 55):
            time = str(h) + ":50:00"
        if (m >= 55 and m < 60):
            if (h + 1) == 24:
                time = str(h) + ":59:59"
            else:
                time = str(h + 1) + ":00:00"
        r.append(time)
    return r





def merge_time_by_weight(index, timeList):
   
    for i in range(len(timeList)):
        length = len(timeList)
        if i == index:
            if index == 0:
                timeList[1][0] = timeList[0][0]
                timeList.remove(timeList[0])

            elif index == length - 1:
                timeList[length - 2][1] = timeList[length - 1][1]
                timeList.remove(timeList[length - 1])

            else:
                timeList[index - 1][1] = timeList[i][1]
                timeList.remove(timeList[index])
    return timeList


def get_Qn(csv_data,start_time,end_time,num_15min):
    total=0
   
    for index, row in csv_data.iterrows():
        row_time=datetime.datetime.strptime(row["时间"], "%H:%M:%S")
       
        if  row_time>=start_time and row_time<=end_time:
            total+=row["相位1"]+row["相位2"]+row["相位3"]
    Qn=float('%.2f' %(total/num_15min))
    return Qn


def merge_time(timeList,n,csv_data):
    while True:
        Qn_list=[]
        min_h=999999999
        min_index=99999999
        min_Qn=999999999
        for i in range(len(timeList)):
            start_time = datetime.datetime.strptime(timeList[i][0], "%H:%M:%S")
            end_time = datetime.datetime.strptime(timeList[i][1][:-1], "%H:%M:%S")
            diff_time = (end_time - start_time).seconds
            num_15min=round(diff_time/15/60)
            Qn_list.append(get_Qn(csv_data,start_time,end_time,num_15min))

        for i in range(len(Qn_list)-1):
            h=float('%.2f' %(abs((Qn_list[i+1]-Qn_list[i])/Qn_list[i])))
            if h<min_h:
                min_h=h
                min_index=i+1
                min_Qn=Qn_list[i+1]
            else:
                if h==min_h :
                    if min_Qn>Qn_list[i+1]:
                        min_index=i+1
                        min_Qn=Qn_list[i+1]
        timeList = merge_time_by_weight(min_index, timeList)
        N = len(timeList)
        if N <= n:
            break
    return timeList



def get_interval_num(row):
    flag = ""
    for i in range(len(p1)):
        if row['相位1'] in p1[i]:
            flag += str(i)
    for i in range(len(p2)):
        if row['相位2'] in p2[i]:
            flag += str(i)
    for i in range(len(p3)):
        if row['相位3'] in p3[i]:
            flag += str(i)
    return flag

def gen_phase(X,Qmax,qmax):
    p1=[]
    base=float('%.2f' % (1/X))
    mul=0+base
    pre_mul=mul

    p = Interval(0, base * qmax)
    p1.append(p)

    if X>2:
        for i in range(1, X - 1):
            pre_mul = mul
            mul = mul + base
            p = Interval(pre_mul * qmax, mul * qmax)
            p1.append(p)

        p=Interval(mul * qmax, 1 * Qmax)
        p1.append(p)
    if X==2:
        p=Interval(pre_mul*qmax, 1 * Qmax)
        p1.append(p)

    return p1

def get_process_row(row,std,means):
    row_list = []
    for i in row:
        if int(i)<=means+2*std:
            row_list.append(i)
    return row_list


def get_process_max_value(csv_data):
    row1=csv_data["相位1"]
    row2 = csv_data["相位2"]
    row3 = csv_data["相位3"]

    qmax1=max(get_process_row(row1,csv_data.std()[0],csv_data.mean()[0]))
    qmax2=max(get_process_row(row2,csv_data.std()[1],csv_data.mean()[1]))
    qmax3=max(get_process_row(row3,csv_data.std()[2],csv_data.mean()[2]))
    return qmax1,qmax2,qmax3


for i in directoryFiles:
    print(i)
    csvFilePath=join(filePath,i)
    resultPath=join(savePath,i).replace("csv","txt")

    
    csv_data = pd.read_csv(csvFilePath,encoding = 'gb18030')

    timeList = []
    pre_flag = ""
    max_row = row_count = sum(1 for row in csv_data.iterrows())
    Qmax1=csv_data.max()[1]
    Qmax2 = csv_data.max()[2]
    Qmax3 = csv_data.max()[3]
    qmax1,qmax2,qmax3=get_process_max_value(csv_data)



    p1=gen_phase(X,Qmax1,qmax1)
    p2=gen_phase(X,Qmax2,qmax2)
    p3=gen_phase(X,Qmax3,qmax3)

    for index,row in csv_data.iterrows():
        flag=get_interval_num(row)
        if index==0:
            timeInterval = []
            timeInterval.append(row["时间"].strip())
            pre_flag=flag
        else:
            if flag==pre_flag:
                if index==max_row-1:
                    timeInterval.append(row["时间"].strip())
                    timeList.append(timeInterval)
            else:
               
                    timeInterval.append(row["时间"].strip())
                    timeList.append(timeInterval)
                    timeInterval = []
                    timeInterval.append(row["时间"].strip())
                    pre_flag=flag
    N = len(timeList)

   

    print(timeList)
    resultList=[]

    if N>n:
        resultList=merge_time(timeList,n,csv_data)
    if N<=n:
        resultList=timeList
    result=[]
    for i in resultList:
        result.append(gen_time(i))

    print(result)
    print('\n')
    print('------------------------------------------------------------------------------------------------------'+'\n')
    with open(resultPath,'w') as writer:
        writer.write ("开始时间    结束时间"+"\n")
        for i in result:
            timeStr=i[0]+"    "+i[1]
            writer.write(timeStr+"\n")

