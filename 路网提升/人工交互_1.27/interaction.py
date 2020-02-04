# -*- coding: utf-8 -*-
"""
Created on Wed May  8 19:40:41 2019

@author: lenovo
"""

# coding=utf-8
from __future__ import absolute_import
from __future__ import print_function

import numpy as np
import matplotlib.pyplot as plt
import os
import sys
import optparse
import subprocess
import random
import sumolib
import traci
import time
import re
import xml.dom.minidom
import xml.etree.ElementTree as ET

if 'SUMO_HOME' in os.environ:
    tools = os.path.join(os.environ['SUMO_HOME'], 'tools')
    sys.path.append(tools)
else:
    sys.exit("please declare environment variable 'SUMO_HOME'")


# we need to import python modules from the $SUMO_HOME/tools directory
try:
    sys.path.append(os.path.join(os.path.dirname(
        __file__), '..', '..', '..', '..', "tools"))  # tutorial in tests
    sys.path.append(os.path.join(os.environ.get("SUMO_HOME", os.path.join(
        os.path.dirname(__file__), "..", "..", "..")), "tools"))  # tutorial in docs
    from sumolib import checkBinary
except ImportError:
    sys.exit(
        "please declare environment variable 'SUMO_HOME' as the root directory of your sumo installation (it should contain folders 'bin', 'tools' and 'docs')")

def get_options():
    optParser = optparse.OptionParser()
    optParser.add_option("--nogui", action="store_true",
                         default=False, help="run the commandline version of sumo")
    options, args = optParser.parse_args()
    return options

def time_print(time_begin):   #输出时间
    time_list = []
    time_init = time.time() - time_begin
    if time_init/3600 > 1:
        time_h = int(time_init/3600)
        time_m = int((time_init-time_h*3600) / 60)
        time_s = int(time_init - time_h * 3600 - time_m * 60)
        time_list.append(str(time_h))
        time_list.append('h ')
        time_list.append(str(time_m))
        time_list.append('m ')

    elif time_init/60 > 1:
        time_m = int(time_init/60)
        time_s = int(time_init - time_m * 60)
        time_list.append(str(time_m))
        time_list.append('m ')
    else:
        time_s = int(time_init)

    time_list.append(str(time_s))
    time_list.append('s')
    time_str = ''.join(time_list)

    print(time_str)

time_begin = time.time()

time_b = time_begin

#  min parameters


def get_traveltime():
    TravelTime =(traci.edge.getLastStepVehicleNumber('1')+
                 traci.edge.getLastStepVehicleNumber('2')+
                 traci.edge.getLastStepVehicleNumber('3')+
                 traci.edge.getLastStepVehicleNumber('4')+
                 traci.edge.getLastStepVehicleNumber('5')+
                 traci.edge.getLastStepVehicleNumber('6')+
                 traci.edge.getLastStepVehicleNumber('7')+
                 traci.edge.getLastStepVehicleNumber('8')+
                 traci.edge.getLastStepVehicleNumber('9')+
                 traci.edge.getLastStepVehicleNumber('10')+
                 traci.edge.getLastStepVehicleNumber('11')+
                 traci.edge.getLastStepVehicleNumber('15'))
    return TravelTime





def evaluate(t_ppp1, t_ppp2, t_ppp3, t_y12, t_y23, t_y31):

    iteration = 0
    step = 0
    TotalTravelTime = 0
    TravelTime = 0
    TravelTimeArray = [TravelTime]

    depart_number = []

    # Pre-trained for the simulation environment
    while step < 600:
        # set the phase of traffic light
        for t in range(1, t_ppp1 + 1):
            traci.trafficlight.setPhase('cluster_node2_node2-1', 0)
            step += 1
            traci.simulationStep()
        for t in range(1, t_y12 + 1):
            traci.trafficlight.setPhase('cluster_node2_node2-1', 1)
            step += 1
            traci.simulationStep()
        for t in range(1, t_ppp2 + 1):
            traci.trafficlight.setPhase('cluster_node2_node2-1', 2)
            step += 1
            traci.simulationStep()
        for t in range(1, t_y23 + 1):
            traci.trafficlight.setPhase('cluster_node2_node2-1', 3)
            step += 1
            traci.simulationStep()
        for t in range(1, t_ppp3 + 1):
            traci.trafficlight.setPhase('cluster_node2_node2-1', 4)
            step += 1
            traci.simulationStep()
        for t in range(1, t_y31 + 1):
            traci.trafficlight.setPhase('cluster_node2_node2-1', 5)
            step += 1
        traci.simulationStep()

    while step < 3600:

        for t1 in range(1, t_ppp1 + 1):
            depart_number.append(traci.simulation.getDepartedNumber())
            TravelTime = get_traveltime()
            TravelTimeArray.append(TravelTime)

            TotalTravelTime += TravelTime

            traci.trafficlight.setPhase('cluster_node2_node2-1', 0)
            step += 1
            traci.simulationStep()

        for t2 in range(1, t_y12 + 1):
            depart_number.append(traci.simulation.getDepartedNumber())
            TravelTime = get_traveltime()
            TravelTimeArray.append(TravelTime)

            TotalTravelTime += TravelTime

            traci.trafficlight.setPhase('cluster_node2_node2-1', 1)
            step += 1
            traci.simulationStep()

        for t3 in range(1, t_ppp2 + 1):
            depart_number.append(traci.simulation.getDepartedNumber())
            TravelTime = get_traveltime()
            TravelTimeArray.append(TravelTime)

            TotalTravelTime += TravelTime

            traci.trafficlight.setPhase('cluster_node2_node2-1', 2)
            step += 1
            traci.simulationStep()

        for t4 in range(1, t_y23 + 1):
            depart_number.append(traci.simulation.getDepartedNumber())
            TravelTime = get_traveltime()
            TravelTimeArray.append(TravelTime)

            TotalTravelTime += TravelTime

            traci.trafficlight.setPhase('cluster_node2_node2-1', 3)
            step += 1
            traci.simulationStep()

        for t5 in range(1, t_ppp3 + 1):
            traci.trafficlight.setPhase('cluster_node2_node2-1', 4)

            TravelTime = get_traveltime()
            TravelTimeArray.append(TravelTime)

            TotalTravelTime += TravelTime

            traci.trafficlight.setPhase('cluster_node2_node2-1', 4)
            step += 1
            traci.simulationStep()

        for t5 in range(1, t_y31 + 1):
            depart_number.append(traci.simulation.getDepartedNumber())
            TravelTime = get_traveltime()
            TravelTimeArray.append(TravelTime)

            TotalTravelTime += TravelTime

            traci.trafficlight.setPhase('cluster_node2_node2-1', 5)
            step += 1
            traci.simulationStep()

        if np.sum(depart_number[-50:]) == 0:
            index = 1
            break

        iteration += 1

    traci.close()
    sys.stdout.flush()
    return np.sum(TravelTimeArray)


if __name__ == "__main__":
    options = get_options()    

    if options.nogui:
        sumoBinary = checkBinary('sumo')
    else:
        sumoBinary = checkBinary('sumo')

    #---------------------------------------
    #输入人工交互配时方案，得到评价结果

    #输入相位1绿灯时间
    phase1_green = 15
    # 输入相位1黄灯时间
    phase1_yellow = 3
    # 输入相位2绿灯时间
    phase2_green = 15
    # 输入相位2黄灯时间
    phase2_yellow = 3
    # 输入相位3绿灯时间s
    phase3_green = 15
    # 输入相位3黄灯时间
    phase3_yellow = 3

    phase = [phase1_green, phase2_green, phase3_green,
             phase1_yellow, phase2_yellow, phase3_yellow]

    traci.start([sumoBinary, "-c", "simulation.sumocfg"])

    traveltime = evaluate(phase1_green, phase2_green, phase3_green,
             phase1_yellow, phase2_yellow, phase3_yellow)

    dom = xml.dom.minidom.parse('output-tripinfos.xml')
    root = dom.documentElement
    itemlist = root.getElementsByTagName('tripinfo')
    n = len(itemlist)
    timeloss = []
    waitingcount = []
    duration = []
    speeds = []
    waitingtime = []

    for i in range(0, n):
        item = itemlist[i]
        # 延误
        tL = item.getAttribute("timeLoss")
        tL_tran = float(tL.replace(',', ''))
        # 停车次数
        wC = item.getAttribute("waitingCount")
        wC_tran = float(wC.replace(',', ''))
        # 通过时间
        dura = item.getAttribute("duration")
        dura_tran = float(dura.replace(',', ''))
        # 速度
        rL = item.getAttribute("routeLength")
        rL_w = float(rL.replace(',', ''))
        speed = rL_w/dura_tran
        # 等待时间
        wT = item.getAttribute("waitingTime")
        wT_tran = float(wT.replace(',', ''))

        timeloss.append(tL_tran)
        waitingcount.append(wC_tran)
        duration.append(dura_tran)
        speeds.append(speed)
        waitingtime.append(wT_tran)

    # 最终方案输出
    print("人工交互方案：")
    print("总周期时间： " + str(sum(phase)) + 's')
    print("-----------------------------------------------------------------------------------------------------------------------------------------")
    print("第一主相位时间：" + str(phase1_green) + 's\n' +
          "第一黄灯时间：" + str(phase1_yellow) + 's\n' +
          "第二主相位时间：" + str(phase2_green) + 's\n' +
          "第二黄灯时间：" + str(phase2_yellow) + 's\n' +
          "第三主相位时间：" + str(phase3_green) + 's\n' +
          "第三黄灯时间：" + str(phase3_yellow))
    print("-----------------------------------------------------------------------------------------------------------------------------------------")

    # 人工交互优化方案评价结果输出
    print("人工交互优化方案评价结果输出:")
    print("单位时间通过车辆数(辆/s):", int(np.min(traveltime)/3600))
    print("-----------------------------------------------------------------------------------------------------------------------------------------")
    print("总延误(s): " + str(round(sum(timeloss), 0)))
    print("总停车次数(次): " + str(int(sum(waitingcount))))
    print( "-----------------------------------------------------------------------------------------------------------------------------------------")
    print("平均速度(m/s): " + str(round(np.mean(speeds), 2)))
    print("平均延误(s/辆)：" + str(round(np.mean(timeloss), 2)))
    print("平均通过时间(s): " + str(round(np.mean(duration), 2)))
    print("平均等待时间(s): " + str(round(np.mean(waitingtime), 2)))

