# -*- coding: utf-8 -*-
"""
Created on Wed May  8 19:40:41 2019
@author: lenovo
"""
# coding=utf-8
from __future__ import absolute_import
from __future__ import print_function
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from multiprocessing import Process, Pool
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
# import xml.etree.ElementTree as ET
# if 'SUMO_HOME' in os.environ:
#     tools = os.path.join(os.environ['SUMO_HOME'], 'tools')
#     sys.path.append(tools)
# else:
#     sys.exit("please declare environment variable 'SUMO_HOME'")
# # we need to import python modules from the $SUMO_HOME/tools directory
# try:
#     sys.path.append(os.path.join(os.path.dirname(
#         __file__), '..', '..', '..', '..', "tools"))  # tutorial in tests
#     sys.path.append(os.path.join(os.environ.get("SUMO_HOME", os.path.join(
#         os.path.dirname(__file__), "..", "..", "..")), "tools"))  # tutorial in docs
#     from sumolib import checkBinary
# except ImportError:
#     sys.exit(
#         "please declare environment variable 'SUMO_HOME' as the root directory of your sumo installation (it should contain folders 'bin', 'tools' and 'docs')")

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
    return time_str
# time_begin = time.time()
# time_b = time_begin
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
def run(t_ppp1,t_ppp2,t_ppp3,t_y12,t_y23,t_y31):
    #traci.gui.setZoom("View #0",700)
    # initialize the parameters
    iteration = 0
    step = 0
    #
    TotalTravelTime = 0
    TravelTime = 0
    TravelTimeArray = [TravelTime]
    depart_number = []
    # Pre-trained for the simulation environment
    while step<600:
        # set the phase of traffic light
        for t in range(1,t_ppp1+1):
            traci.trafficlight.setPhase('cluster_node2_node2-1',0)
            step += 1
            traci.simulationStep()
        for t in range(1,t_y12+1):
            traci.trafficlight.setPhase('cluster_node2_node2-1',1)
            step += 1
            traci.simulationStep()
        for t in range(1,t_ppp2+1):
            traci.trafficlight.setPhase('cluster_node2_node2-1',2)
            step += 1
            traci.simulationStep()
        for t in range(1,t_y23+1):
            traci.trafficlight.setPhase('cluster_node2_node2-1',3)
            step += 1
            traci.simulationStep()
        for t in range(1,t_ppp3+1):
            traci.trafficlight.setPhase('cluster_node2_node2-1',4)
            step += 1
            traci.simulationStep()
        for t in range(1,t_y31+1):
            traci.trafficlight.setPhase('cluster_node2_node2-1',5)
            step += 1
        traci.simulationStep()
    index = 0
    #
    while step < 3600:
        for t1 in range(1,t_ppp1+1):
            depart_number.append(traci.simulation.getDepartedNumber())
            TravelTime = get_traveltime() 
            TravelTimeArray.append(TravelTime)
            TotalTravelTime += TravelTime
            traci.trafficlight.setPhase('cluster_node2_node2-1',0)
            step += 1
            traci.simulationStep()
        for t2 in range(1,t_y12+1):
            depart_number.append(traci.simulation.getDepartedNumber())
            TravelTime =get_traveltime()
            TravelTimeArray.append(TravelTime)
            TotalTravelTime += TravelTime
            traci.trafficlight.setPhase('cluster_node2_node2-1',1)
            step += 1
            traci.simulationStep()
        for t3 in range(1,t_ppp2+1):
            depart_number.append(traci.simulation.getDepartedNumber())
            TravelTime =get_traveltime()  
            TravelTimeArray.append(TravelTime)
            TotalTravelTime += TravelTime
            traci.trafficlight.setPhase('cluster_node2_node2-1',2)
            step += 1
            traci.simulationStep()
        for t4 in range(1,t_y23+1):
            depart_number.append(traci.simulation.getDepartedNumber())
            TravelTime =get_traveltime()
            TravelTimeArray.append(TravelTime)
            TotalTravelTime += TravelTime
            traci.trafficlight.setPhase('cluster_node2_node2-1',3)
            step += 1
            traci.simulationStep()
        for t5 in range(1,t_ppp3+1):
            traci.trafficlight.setPhase('cluster_node2_node2-1',4)
            TravelTime =get_traveltime()
            TravelTimeArray.append(TravelTime)
            TotalTravelTime += TravelTime
            traci.trafficlight.setPhase('cluster_node2_node2-1',4)
            step += 1
            traci.simulationStep()
        for t5 in range(1,t_y31+1):
            depart_number.append(traci.simulation.getDepartedNumber())
            TravelTime =get_traveltime()
            TravelTimeArray.append(TravelTime) 
            TotalTravelTime += TravelTime
            traci.trafficlight.setPhase('cluster_node2_node2-1',5)
            step += 1
            traci.simulationStep()
        if np.sum(depart_number[-50 :]) == 0 :
            index = 1
            break
        iteration += 1
    traci.close()
    sys.stdout.flush()     
    return TravelTimeArray,TotalTravelTime,index
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
class SinglePointSimulation():
    def _init_(self,min_greentimme,max_greentimme,interval):
        self.min_greentimme=min_greentimme
        self.max_greentimme=max_greentimme
        self.interval=interval
        P=Process(target=self.run)
        return P.start()
    def run(self):
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
        time_begin = time.time()
        time_b = time_begin
        # 输入最小绿灯时间
        min_greentimme = self.min_greentimme
        # 输入最大绿灯时间
        max_greentimme = self.max_greentimme
        maxmum = max_greentimme - min_greentimme
        # 输入迭代时间间隔
        interval = self.interval
        # 迭代次数
        n = round(maxmum/interval)
        # 输入最小周期时间（由最小流程等时间算得）
        min_cycletime = min_greentimme*3 + 3*3
        # 输入最大周期时间（由最大、最小绿灯时间算得）
        max_cycletime = maxmum + min_greentimme*3 + 3*3
        # 输入迭代时间间隔
        interval = 10
        options = get_options()    
        # if options.nogui:
        sumoBinary = checkBinary('sumo')
        # else:
        #     sumoBinary = checkBinary('sumo-gui')
        # number of action sets and features
        simulation_time = 0
        Game_Name = "Cross"
        RewardMatrix = []
        EpisodeTotalTravelTime = 0
        TravelTimeStep = []
        TravelTimeMatrix = []
        SystemTravelTime = []
        SUMOTravelTime = []
        P = []
        # this is the normal way of using traci. sumo is started as a
        # subprocess and then the pytho  n script connects and runs
        for i in range(0,n):
            for j in range(0,n-i):
                k=0
                string='10s迭代'
                while k < n-i-j:
                    traci.start([sumoBinary, "-c", os.path.dirname(os.path.abspath(__file__))+"/simulation.sumocfg"])
                    tp1 = i*interval + min_greentimme
                    tp2 = j*interval + min_greentimme
                    tp3 = k*interval + min_greentimme
                    TravelTimeStep,EpisodeTotalTravelTime,Index = run(tp1,tp2,tp3,3,3,3)
                    if Index == 0:
                        simulation_time += 1
                        TravelTimeMatrix.append(TravelTimeStep)
                        SystemTravelTime.append(EpisodeTotalTravelTime)
                        p = [tp1,tp2,tp3]
                        P.append(p)
                        # 输出每一次迭代的训练日志
                        file=open(string+"第"+str(simulation_time)+"次仿真："+'日志.txt','w')
                        file.writelines("-----------------------------------------------------------------------------------------------------------------------------------------"+'\n')
                        file.writelines(str("第"+str(simulation_time)+"次仿真："))
                        file.writelines("第一主相位时间："+str(tp1)+'s\n'+
                            "第二主相位时间："+str(tp2)+'s\n'+
                            "第三主相位时间："+str(tp3)+'s\n'+
                            "第一黄灯时间："+str(3)+'s\n'+
                            "第二黄灯时间："+str(3)+'s\n'+
                            "第三黄灯时间："+str(3))
                        
                        file.writelines("本次仿真时间：")
                        file.writelines(str(time_print(time_b)))
                        file.writelines("总仿真时间：")
                        file.writelines(str(time_print(time_begin)))
                        file.writelines(str("单位时间通过车辆数："+str(int(EpisodeTotalTravelTime / 3600))+"veh/s")+'\n')
                        file.writelines("-----------------------------------------------------------------------------------------------------------------------------------------"+'\n')

                        print("-----------------------------------------------------------------------------------------------------------------------------------------")
                        print("第"+str(simulation_time)+"次仿真：")
                        print("第一主相位时间："+str(tp1)+'s\n'+
                            "第二主相位时间："+str(tp2)+'s\n'+
                            "第三主相位时间："+str(tp3)+'s\n'+
                            "第一黄灯时间："+str(3)+'s\n'+
                            "第二黄灯时间："+str(3)+'s\n'+
                            "第三黄灯时间："+str(3))
                        print("本次仿真时间：")
                        print(time_print(time_b))
                        print("总仿真时间：")
                        print(time_print(time_begin))
                        print("单位时间通过车辆数：", int(EpisodeTotalTravelTime / 3600), "veh/s")
                        print("-----------------------------------------------------------------------------------------------------------------------------------------")
                        time_b = time.time()
                        k += 1
                    else:
                        file.writelines("-----------------------------------------------------------------------------------------------------------------------------------------"+'\n')
                        file.writelines("本次损失时间")
                        file.writelines(time_print(time_b))
                        file.writelines("总运行时间：")
                        file.writelines(time_print(time_begin))
                        file.writelines("-----------------------------------------------------------------------------------------------------------------------------------------"+'\n')
                        
                        print("-----------------------------------------------------------------------------------------------------------------------------------------")
                        print("本次损失时间")
                        print(time_print(time_b))
                        print("总运行时间：")
                        print(time_print(time_begin))
                        print("-----------------------------------------------------------------------------------------------------------------------------------------")
                        time_b = time.time()
                        k = k-1
        # Travel time calculated by me
        SysTimePlot = np.array(SystemTravelTime)
        plt.plot(np.arange(len(SystemTravelTime)), SystemTravelTime)
        plt.ylabel('System Travel Time')
        plt.xlabel('Episodes')
        plt.savefig('1.png')
        
        plt.close()
        # print the best tp we found
        [ best_10s_1 , best_10s_2 , best_10s_3 ] = P[SystemTravelTime.index(np.min(SystemTravelTime))]

        file=open("10s迭代的最终输出结果.txt",'w')
        # 输出10s迭代的最终输出结果
        file.writelines("以%ds为间隔迭代的初始最优结果为：" % interval)
        file.writelines("第一主相位时间："+str(best_10s_1)+"第二主相位时间："+str(best_10s_2)+"第三主相位时间："+str(best_10s_3)+"第一黄灯时间："+str(3)+"第二黄灯时间："+str(3)+"第三黄灯时间："+str(3))
        print("以%ds为间隔迭代的初始最优结果为：" % interval)
        print("第一主相位时间："+str(best_10s_1)+"第二主相位时间："+str(best_10s_2)+"第三主相位时间："+str(best_10s_3)+"第一黄灯时间："+str(3)+"第二黄灯时间："+str(3)+"第三黄灯时间："+str(3))
        #------------------------------------------------------
        # 开始以1s为间隔迭代，直到得到最终最优结果
        simulation_time1 = 0
        Game_Name1 = "Cross"
        RewardMatrix1 = []
        EpisodeTotalTravelTime1 = 0
        TravelTimeStep1 = []
        TravelTimeMatrix1 = []
        SystemTravelTime1 = []
        SUMOTravelTime1 = []
        [best_5s_1, best_5s_2, best_5s_3] = [best_10s_1, best_10s_2, best_10s_3]
        # P[SystemTravelTime.index(np.min(SystemTravelTime))]
        P1 = []
        # this is the normal way of using traci. sumo is started as a
        # subprocess and then the python script connects and runs
        for i in range(0, 10):
            k = 0
            string='1s迭代'
            while k < 10:
                traci.start([sumoBinary, "-c", os.path.dirname(os.path.abspath(__file__))+"/simulation.sumocfg"])
                tp1 = i + best_5s_1
                tp2 = best_5s_2
                tp3 = k + best_5s_3
                TravelTimeStep, EpisodeTotalTravelTime, Index = run(tp1, tp2, tp3, 3, 3, 3)
                if Index == 0:
                    simulation_time1 += 1
                    TravelTimeMatrix1.append(TravelTimeStep)
                    SystemTravelTime1.append(EpisodeTotalTravelTime)
                    p = [tp1, tp2, tp3]
                    P1.append(p)
                    
                    file=open(string+"第"+str(simulation_time)+"次仿真："+'日志.txt','w')
                    file.writelines(
                        "-----------------------------------------------------------------------------------------------------------------------------------------")
                    file.writelines(str("第"+str(simulation_time1)+"次仿真："))
                    file.writelines("第一主相位时间：" + str(tp1) + 's\n' +
                        "第二主相位时间：" + str(tp2) + 's\n' +
                        "第三主相位时间：" + str(tp3) + 's\n' +
                        "第一黄灯时间：" + str(3) + 's\n' +
                        "第二黄灯时间：" + str(3) + 's\n' +
                        "第三黄灯时间：" + str(3))
                    file.writelines("本次仿真时间：")
                    file.writelines(time_print(time_b))
                    file.writelines("总仿真时间：")
                    file.writelines(time_print(time_begin))
                    file.writelines(str("单位时间通过车辆数："+str(int(EpisodeTotalTravelTime / 3600))+"veh/s")+'\n')
                    file.writelines(
                        "-----------------------------------------------------------------------------------------------------------------------------------------")
                   
                    print(
                        "-----------------------------------------------------------------------------------------------------------------------------------------")
                    print(string+"第", simulation_time1, "次仿真：")
                    print("第一主相位时间：" + str(tp1) + 's\n' +
                        "第二主相位时间：" + str(tp2) + 's\n' +
                        "第三主相位时间：" + str(tp3) + 's\n' +
                        "第一黄灯时间：" + str(3) + 's\n' +
                        "第二黄灯时间：" + str(3) + 's\n' +
                        "第三黄灯时间：" + str(3))
                    print("本次仿真时间：")
                    print(time_print(time_b))
                    print("总仿真时间：")
                    print(time_print(time_begin))
                    print("单位时间通过车辆数：", int(EpisodeTotalTravelTime / 3600), "veh/s")
                    print(
                        "-----------------------------------------------------------------------------------------------------------------------------------------")
                    time_b = time.time()
                    k += 1
                else:
                    file.writelines("-----------------------------------------------------------------------------------------------------------------------------------------"+'\n')
                    file.writelines("本次损失时间")
                    file.writelines(time_print(time_b))
                    file.writelines("总运行时间：")
                    file.writelines(time_print(time_begin))
                    file.writelines("-----------------------------------------------------------------------------------------------------------------------------------------"+'\n')
                        
                    print(
                        "-----------------------------------------------------------------------------------------------------------------------------------------")
                    print("本次损失时间")
                    print(time_print(time_b))
                    print("总运行时间：")
                    print(time_print(time_begin))
                    print(
                        "-----------------------------------------------------------------------------------------------------------------------------------------")
                    time_b = time.time()
                    k = k - 1
        # Travel time calculated by me
        SysTimePlot = np.array(SystemTravelTime1)
        plt.plot(np.arange(len(SystemTravelTime1)), SystemTravelTime1)
        plt.ylabel('System Travel Time')
        plt.xlabel('Episodes')
        
        plt.savefig('2.png')
        plt.close()
        best_index = np.argpartition(SystemTravelTime1, 10)
        for i in range(1, 11):
            [best_1s_1, best_1s_2, best_1s_3] = P1[SystemTravelTime1.index(np.min(SystemTravelTime1))]
        [best_1s_1, best_1s_2, best_1s_3] = P1[SystemTravelTime1.index(np.min(SystemTravelTime1))]
        phasetime = [best_1s_1, best_1s_2, best_1s_3,3,3,3]
        file=open("optimisticDuring.txt",'w')
        file.writelines("定时周期优化最优方案："+'\n')
        file.writelines("总周期时间： " + str(sum(phasetime)) + 's')
        file.writelines(str("第一主相位时间：" + str(best_1s_1) + 's\n' +
            "第一黄灯时间：" + str(3) + 's\n' +
            "第二主相位时间：" + str(best_1s_2) + 's\n' +
            "第二黄灯时间：" + str(3) + 's\n' +
            "第三主相位时间：" + str(best_1s_3) + 's\n' +
            "第三黄灯时间：" + str(3)))

        print("定时周期优化最优方案：")
        print("总周期时间： " + str(sum(phasetime)) + 's')
        print("第一主相位时间：" + str(best_1s_1) + 's\n' +
            "第一黄灯时间：" + str(3) + 's\n' +
            "第二主相位时间：" + str(best_1s_2) + 's\n' +
            "第二黄灯时间：" + str(3) + 's\n' +
            "第三主相位时间：" + str(best_1s_3) + 's\n' +
            "第三黄灯时间：" + str(3))
        #---------------------------------------
        #运行最佳配时方案，得到评价结果
        traci.start([sumoBinary, "-c", os.path.dirname(os.path.abspath(__file__))+"/simulation.sumocfg"])
        evaluate(best_1s_1, best_1s_2, best_1s_3,3,3,3)
        dom = xml.dom.minidom.parse(os.path.dirname(os.path.abspath(__file__))+'/output-tripinfos.xml')
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
        file=open("最终方案输出.txt",'w')
        # 最终方案输出
        file.writelines("定时周期优化最优方案：")
        file.writelines("总周期时间： " + str(sum(phasetime)) + 's\n')
        file.writelines("-----------------------------------------------------------------------------------------------------------------------------------------"+'\n')
        file.writelines("第一主相位时间：" + str(best_1s_1) + 's\n' +
            "第一黄灯时间：" + str(3) + 's\n' +
            "第二主相位时间：" + str(best_1s_2) + 's\n' +
            "第二黄灯时间：" + str(3) + 's\n' +
            "第三主相位时间：" + str(best_1s_3) + 's\n' +
            "第三黄灯时间：" + str(3))
        file.writelines("-----------------------------------------------------------------------------------------------------------------------------------------"+'\n')
        
        print("定时周期优化最优方案：")
        print("总周期时间： " + str(sum(phasetime)) + 's')
        print("-----------------------------------------------------------------------------------------------------------------------------------------")
        print("第一主相位时间：" + str(best_1s_1) + 's\n' +
            "第一黄灯时间：" + str(3) + 's\n' +
            "第二主相位时间：" + str(best_1s_2) + 's\n' +
            "第二黄灯时间：" + str(3) + 's\n' +
            "第三主相位时间：" + str(best_1s_3) + 's\n' +
            "第三黄灯时间：" + str(3))
        print("-----------------------------------------------------------------------------------------------------------------------------------------")
        file=open("最终方案评价结果输出.txt",'w')
        # 最终方案评价结果输出
        file.writelines("最终方案评价结果输出:"+'\n')
        file.writelines("单位时间通过车辆数(辆/s):"+ str(int(np.min(SystemTravelTime1)/3600))+'\n')
        file.writelines("-----------------------------------------------------------------------------------------------------------------------------------------"+'\n')
        file.writelines("总延误(s): " + str(round(sum(timeloss), 0))+'\n')
        file.writelines("总停车次数(次): " + str(int(sum(waitingcount)))+'\n')
        file.writelines( "-----------------------------------------------------------------------------------------------------------------------------------------"+'\n')
        file.writelines("平均速度(m/s): " + str(round(np.mean(speeds),2))+'\n')
        file.writelines("平均延误(s/辆)：" + str(round(np.mean(timeloss),2))+'\n')
        file.writelines("平均通过时间(s): " + str(round(np.mean(duration),2))+'\n')

        print("最终方案评价结果输出:")
        print("单位时间通过车辆数(辆/s):", int(np.min(SystemTravelTime1)/3600))
        print("-----------------------------------------------------------------------------------------------------------------------------------------")
        print("总延误(s): " + str(round(sum(timeloss), 0)))
        print("总停车次数(次): " + str(int(sum(waitingcount))))
        print( "-----------------------------------------------------------------------------------------------------------------------------------------")
        print("平均速度(m/s): " + str(round(np.mean(speeds), 2)))
        print("平均延误(s/辆)：" + str(round(np.mean(timeloss), 2)))
        print("平均通过时间(s): " + str(round(np.mean(duration), 2)))
        print("平均等待时间(s): " + str(round(np.mean(waitingtime), 2)))
        return 1

if __name__ == "__main__":
    simulation=SinglePointSimulation()
    simulation._init_(15,95,10)
    # # 输入最小绿灯时间
    # min_greentimme = 15
    # # 输入最大绿灯时间
    # max_greentimme = 95
    # maxmum = max_greentimme - min_greentimme
    # # 输入迭代时间间隔
    # interval = 10
    # # 迭代次数
    # n = round(maxmum/interval)
    # # 输入最小周期时间（由最小流程等时间算得）
    # min_cycletime = min_greentimme*3 + 3*3
    # # 输入最大周期时间（由最大、最小绿灯时间算得）
    # max_cycletime = maxmum + min_greentimme*3 + 3*3
    # # 输入迭代时间间隔
    # interval = 10
    # options = get_options()    
    # # if options.nogui:
    # sumoBinary = checkBinary('sumo')
    # # else:
    # #     sumoBinary = checkBinary('sumo-gui')
    # # number of action sets and features
    # simulation_time = 0
    # Game_Name = "Cross"
    # RewardMatrix = []
    # EpisodeTotalTravelTime = 0
    # TravelTimeStep = []
    # TravelTimeMatrix = []
    # SystemTravelTime = []
    # SUMOTravelTime = []
    # P = []
    # # this is the normal way of using traci. sumo is started as a
    # # subprocess and then the pytho  n script connects and runs
    # for i in range(0,n):
    #     for j in range(0,n-i):
    #         k=0
    #         while k < n-i-j:
    #             traci.start([sumoBinary, "-c", os.path.dirname(os.path.abspath(__file__))+"/simulation.sumocfg"])
    #             tp1 = i*interval + min_greentimme
    #             tp2 = j*interval + min_greentimme
    #             tp3 = k*interval + min_greentimme
    #             TravelTimeStep,EpisodeTotalTravelTime,Index = run(tp1,tp2,tp3,3,3,3)
    #             if Index == 0:
    #                 simulation_time += 1
    #                 TravelTimeMatrix.append(TravelTimeStep)
    #                 SystemTravelTime.append(EpisodeTotalTravelTime)
    #                 p = [tp1,tp2,tp3]
    #                 P.append(p)
    #                 # 输出每一次迭代的训练日志
    #                 print("-----------------------------------------------------------------------------------------------------------------------------------------")
    #                 print("第",simulation_time,"次仿真：")
    #                 print("第一主相位时间："+str(tp1)+'s\n'+
    #                       "第二主相位时间："+str(tp2)+'s\n'+
    #                       "第三主相位时间："+str(tp3)+'s\n'+
    #                       "第一黄灯时间："+str(3)+'s\n'+
    #                       "第二黄灯时间："+str(3)+'s\n'+
    #                       "第三黄灯时间："+str(3))
    #                 print("本次仿真时间：")
    #                 time_print(time_b)
    #                 print("总仿真时间：")
    #                 time_print(time_begin)
    #                 print("单位时间通过车辆数：", int(EpisodeTotalTravelTime / 3600), "veh/s")
    #                 print("-----------------------------------------------------------------------------------------------------------------------------------------")
    #                 time_b = time.time()
    #                 k += 1
    #             else:
    #                 print("-----------------------------------------------------------------------------------------------------------------------------------------")
    #                 print("本次损失时间")
    #                 time_print(time_b)
    #                 print("总运行时间：")
    #                 time_print(time_begin)
    #                 print("-----------------------------------------------------------------------------------------------------------------------------------------")
    #                 time_b = time.time()
    #                 k = k-1
    # # Travel time calculated by me
    # SysTimePlot = np.array(SystemTravelTime)
    # plt.plot(np.arange(len(SystemTravelTime)), SystemTravelTime)
    # plt.ylabel('System Travel Time')
    # plt.xlabel('Episodes')
    # 
    # plt.close()
    # # print the best tp we found
    # [ best_10s_1 , best_10s_2 , best_10s_3 ] = P[SystemTravelTime.index(np.min(SystemTravelTime))]
    # # 输出10s迭代的最终输出结果
    # print("以%ds为间隔迭代的初始最优结果为：" % interval)
    # print("第一主相位时间："+str(best_10s_1)+"第二主相位时间："+str(best_10s_2)+"第三主相位时间："+str(best_10s_3)+"第一黄灯时间："+str(3)+"第二黄灯时间："+str(3)+"第三黄灯时间："+str(3))
    # #------------------------------------------------------
    # # 开始以1s为间隔迭代，直到得到最终最优结果
    # simulation_time1 = 0
    # Game_Name1 = "Cross"
    # RewardMatrix1 = []
    # EpisodeTotalTravelTime1 = 0
    # TravelTimeStep1 = []
    # TravelTimeMatrix1 = []
    # SystemTravelTime1 = []
    # SUMOTravelTime1 = []
    # [best_5s_1, best_5s_2, best_5s_3] = [best_10s_1, best_10s_2, best_10s_3]
    # # P[SystemTravelTime.index(np.min(SystemTravelTime))]
    # P1 = []
    # # this is the normal way of using traci. sumo is started as a
    # # subprocess and then the python script connects and runs
    # for i in range(0, 10):
    #     k = 0
    #     while k < 10:
    #         traci.start([sumoBinary, "-c", os.path.dirname(os.path.abspath(__file__))+"/simulation.sumocfg"])
    #         tp1 = i + best_5s_1
    #         tp2 = best_5s_2
    #         tp3 = k + best_5s_3
    #         TravelTimeStep, EpisodeTotalTravelTime, Index = run(tp1, tp2, tp3, 3, 3, 3)
    #         if Index == 0:
    #             simulation_time1 += 1
    #             TravelTimeMatrix1.append(TravelTimeStep)
    #             SystemTravelTime1.append(EpisodeTotalTravelTime)
    #             p = [tp1, tp2, tp3]
    #             P1.append(p)
    #             print(
    #                 "-----------------------------------------------------------------------------------------------------------------------------------------")
    #             print("第", simulation_time1, "次仿真：")
    #             print("第一主相位时间：" + str(tp1) + 's\n' +
    #                   "第二主相位时间：" + str(tp2) + 's\n' +
    #                   "第三主相位时间：" + str(tp3) + 's\n' +
    #                   "第一黄灯时间：" + str(3) + 's\n' +
    #                   "第二黄灯时间：" + str(3) + 's\n' +
    #                   "第三黄灯时间：" + str(3))
    #             print("本次仿真时间：")
    #             time_print(time_b)
    #             print("总仿真时间：")
    #             time_print(time_begin)
    #             print("单位时间通过车辆数：", int(EpisodeTotalTravelTime / 3600), "veh/s")
    #             print(
    #                 "-----------------------------------------------------------------------------------------------------------------------------------------")
    #             time_b = time.time()
    #             k += 1
    #         else:
    #             print(
    #                 "-----------------------------------------------------------------------------------------------------------------------------------------")
    #             print("本次损失时间")
    #             time_print(time_b)
    #             print("总运行时间：")
    #             time_print(time_begin)
    #             print(
    #                 "-----------------------------------------------------------------------------------------------------------------------------------------")
    #             time_b = time.time()
    #             k = k - 1
    # # Travel time calculated by me
    # SysTimePlot = np.array(SystemTravelTime1)
    # plt.plot(np.arange(len(SystemTravelTime1)), SystemTravelTime1)
    # plt.ylabel('System Travel Time')
    # plt.xlabel('Episodes')
    # 
    # plt.savefig('output.png')
    # plt.close()
    # best_index = np.argpartition(SystemTravelTime1, 10)
    # for i in range(1, 11):
    #     [best_1s_1, best_1s_2, best_1s_3] = P1[SystemTravelTime1.index(np.min(SystemTravelTime1))]
    # [best_1s_1, best_1s_2, best_1s_3] = P1[SystemTravelTime1.index(np.min(SystemTravelTime1))]
    # phasetime = [best_1s_1, best_1s_2, best_1s_3,3,3,3]
    # print("定时周期优化最优方案：")
    # print("总周期时间： " + str(sum(phasetime)) + 's')
    # print("第一主相位时间：" + str(best_1s_1) + 's\n' +
    #       "第一黄灯时间：" + str(3) + 's\n' +
    #       "第二主相位时间：" + str(best_1s_2) + 's\n' +
    #       "第二黄灯时间：" + str(3) + 's\n' +
    #       "第三主相位时间：" + str(best_1s_3) + 's\n' +
    #       "第三黄灯时间：" + str(3))
    # #---------------------------------------
    # #运行最佳配时方案，得到评价结果
    # traci.start([sumoBinary, "-c", os.path.dirname(os.path.abspath(__file__))+"/simulation.sumocfg"])
    # evaluate(best_1s_1, best_1s_2, best_1s_3,3,3,3)
    # dom = xml.dom.minidom.parse('output-tripinfos.xml')
    # root = dom.documentElement
    # itemlist = root.getElementsByTagName('tripinfo')
    # n = len(itemlist)
    # timeloss = []
    # waitingcount = []
    # duration = []
    # speeds = []
    # waitingtime = []
    # for i in range(0, n):
    #     item = itemlist[i]
    #     # 延误
    #     tL = item.getAttribute("timeLoss")
    #     tL_tran = float(tL.replace(',', ''))
    #     # 停车次数
    #     wC = item.getAttribute("waitingCount")
    #     wC_tran = float(wC.replace(',', ''))
    #     # 通过时间
    #     dura = item.getAttribute("duration")
    #     dura_tran = float(dura.replace(',', ''))
    #     # 速度
    #     rL = item.getAttribute("routeLength")
    #     rL_w = float(rL.replace(',', ''))
    #     speed = rL_w/dura_tran
    #     # 等待时间
    #     wT = item.getAttribute("waitingTime")
    #     wT_tran = float(wT.replace(',', ''))
    #     timeloss.append(tL_tran)
    #     waitingcount.append(wC_tran)
    #     duration.append(dura_tran)
    #     speeds.append(speed)
    #     waitingtime.append(wT_tran)
    # # 最终方案输出
    # print("定时周期优化最优方案：")
    # print("总周期时间： " + str(sum(phasetime)) + 's')
    # print("-----------------------------------------------------------------------------------------------------------------------------------------")
    # print("第一主相位时间：" + str(best_1s_1) + 's\n' +
    #       "第一黄灯时间：" + str(3) + 's\n' +
    #       "第二主相位时间：" + str(best_1s_2) + 's\n' +
    #       "第二黄灯时间：" + str(3) + 's\n' +
    #       "第三主相位时间：" + str(best_1s_3) + 's\n' +
    #       "第三黄灯时间：" + str(3))
    # print("-----------------------------------------------------------------------------------------------------------------------------------------")
    # # 最终方案评价结果输出
    # print("最终方案评价结果输出:")
    # print("单位时间通过车辆数(辆/s):", int(np.min(SystemTravelTime1)/3600))
    # print("-----------------------------------------------------------------------------------------------------------------------------------------")
    # print("总延误(s): " + str(round(sum(timeloss), 0)))
    # print("总停车次数(次): " + str(int(sum(waitingcount))))
    # print( "-----------------------------------------------------------------------------------------------------------------------------------------")
    # print("平均速度(m/s): " + str(round(np.mean(speeds), 2)))
    # print("平均延误(s/辆)：" + str(round(np.mean(timeloss), 2)))
    # print("平均通过时间(s): " + str(round(np.mean(duration), 2)))
    # print("平均等待时间(s): " + str(round(np.mean(waitingtime), 2)))
