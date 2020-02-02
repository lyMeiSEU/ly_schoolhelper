# -*- coding: utf-8 -*-
"""
Created on Tue May 14 20:33:45 2019

@author: DD
"""

# coding=utf-8
from __future__ import absolute_import
from __future__ import print_function

from Q_table import QLearningTable
import numpy as np
import matplotlib.pyplot as plt
import os
import sys
import optparse
import subprocess
import random
import traci
import time
import re


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
    from sumolib import checkBinary  # noqa
except ImportError:
    sys.exit(
        "please declare environment variable 'SUMO_HOME' as the root directory of your sumo installation (it should contain folders 'bin', 'tools' and 'docs')")

def get_options():
    optParser = optparse.OptionParser()
    optParser.add_option("--nogui", action="store_true",
                         default=False, help="run the commandline version of sumo")
    options, args = optParser.parse_args()
    return options

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




def get_flow1():
    flow1 = (traci.inductionloop.getLastStepVehicleNumber('D6_1_0')+                     
             traci.inductionloop.getLastStepVehicleNumber('D6_1_1')+
             traci.inductionloop.getLastStepVehicleNumber('D6_1_2')+
             traci.inductionloop.getLastStepVehicleNumber('D6_1_3'))
    return flow1

def get_flow2():
    flow2 = (traci.inductionloop.getLastStepVehicleNumber('D4_1_0')+                     
             traci.inductionloop.getLastStepVehicleNumber('D4_1_1'))
    return flow2

def get_flow3():
    flow3 = (traci.inductionloop.getLastStepVehicleNumber('D8_1_0')+                     
             traci.inductionloop.getLastStepVehicleNumber('D8_1_1')+
             traci.inductionloop.getLastStepVehicleNumber('D8_1_2'))
    return flow3

def get_flow4():
    flow4 = (traci.inductionloop.getLastStepVehicleNumber('D1_1_0'))
    return flow4

def get_flow():
    flow = get_flow1() + get_flow2() + get_flow3() + get_flow4() 
    return flow


def get_flowp1():
    flowp1 = (traci.inductionloop.getLastStepVehicleNumber('D6_1_3')+                     
             traci.inductionloop.getLastStepVehicleNumber('D6_1_1')+
             traci.inductionloop.getLastStepVehicleNumber('D6_1_2')+
             traci.inductionloop.getLastStepVehicleNumber('D8_1_0')+
             traci.inductionloop.getLastStepVehicleNumber('D8_1_0'))
    return flowp1

def get_flowp2():
    flowp2 = (traci.inductionloop.getLastStepVehicleNumber('D6_1_3')+                     
             traci.inductionloop.getLastStepVehicleNumber('D6_1_0')+
             traci.inductionloop.getLastStepVehicleNumber('D8_1_0')+
             traci.inductionloop.getLastStepVehicleNumber('D8_1_2'))
    return flowp2

def get_flowp3():
    flowp3 = (traci.inductionloop.getLastStepVehicleNumber('D1_1_0')+
            traci.inductionloop.getLastStepVehicleNumber('D4_1_0')+
            traci.inductionloop.getLastStepVehicleNumber('D4_1_1')+
            traci.inductionloop.getLastStepVehicleNumber('D6_1_0')+
            traci.inductionloop.getLastStepVehicleNumber('D8_1_0')
            )
    return flowp3


def get_nump1():
    nump1=(traci.lane.getLastStepVehicleNumber('15_0')+traci.lane.getLastStepVehicleNumber('15_1')+traci.lane.getLastStepVehicleNumber('15_2')   #南北直右
        +traci.lane.getLastStepVehicleNumber('8_0')+traci.lane.getLastStepVehicleNumber('8_1')
        +traci.lane.getLastStepVehicleNumber('6_0')+traci.lane.getLastStepVehicleNumber('6_1')+traci.lane.getLastStepVehicleNumber('6_2'))
    return nump1

def get_nump2():
    nump2=(traci.lane.getLastStepVehicleNumber('15_0')+traci.lane.getLastStepVehicleNumber('15_1')+traci.lane.getLastStepVehicleNumber('15_2')+
        traci.lane.getLastStepVehicleNumber('6_0')+traci.lane.getLastStepVehicleNumber('6_3')+
        traci.lane.getLastStepVehicleNumber('8_0')+traci.lane.getLastStepVehicleNumber('8_2'))    #南北左右
    return nump2

def get_nump3():
    nump3=(traci.lane.getLastStepVehicleNumber('1_0')           #东西直左右
        +traci.lane.getLastStepVehicleNumber('4_0')
        +traci.lane.getLastStepVehicleNumber('4_1'))
    return nump3

def calcreward_step():
    reward_step = get_flow()
    return reward_step

def calcreward(Reward_step):
    reward = np.sum( Reward_step[-3:] )
    return reward

# Extract the image 方案三部分
def CalcState(T):
    
    s1=(traci.lane.getLastStepVehicleNumber('15_0')+traci.lane.getLastStepVehicleNumber('15_1')+traci.lane.getLastStepVehicleNumber('15_2')+
        traci.lane.getLastStepVehicleNumber('6_0')+traci.lane.getLastStepVehicleNumber('6_3')+
        traci.lane.getLastStepVehicleNumber('8_0')+traci.lane.getLastStepVehicleNumber('8_2'))    #南北左右
    s2=(traci.lane.getLastStepVehicleNumber('15_0')+traci.lane.getLastStepVehicleNumber('15_1')+traci.lane.getLastStepVehicleNumber('15_2')   #南北直右
        +traci.lane.getLastStepVehicleNumber('8_0')+traci.lane.getLastStepVehicleNumber('8_1')
        +traci.lane.getLastStepVehicleNumber('6_0')+traci.lane.getLastStepVehicleNumber('6_1')+traci.lane.getLastStepVehicleNumber('6_2'))
    s3=(traci.lane.getLastStepVehicleNumber('1_0')           #东西直左右
        +traci.lane.getLastStepVehicleNumber('4_0')
        +traci.lane.getLastStepVehicleNumber('4_1'))
    
    s4 = T

    State = np.array([s1,s2,s3,s4])   # n维状态集
    
    State = str(State)

    # [上游密度、交织区密度、匝道密度、信号灯色]
    return State

def a_step(departed , departed_number , TravelTimeArray , TotalTravelTime , Reward_step):
    
    traci.simulationStep()
    
    departed += traci.simulation.getDepartedNumber()
    
    departed_number.append(traci.simulation.getDepartedNumber())
    
    TravelTime = get_traveltime()
    
    TravelTimeArray.append(TravelTime)
    
    TotalTravelTime += TravelTime
    
    Reward_step.append( calcreward_step() )    

#    print(traci.trafficlight.getPhase('cluster_node2_node2-1'))

    return departed , departed_number , TravelTimeArray , TotalTravelTime , Reward_step

def run(i, number_of_simulation):
    #traci.gui.setZoom("View #0",700)
    # initialize the parameters
    step = 0
    iteration = 0
    Reward_step = []
    Reward = []
    Action = []
    TA = []
    FLOW = []
    P = []

    
    #最小绿灯时间
    min_green_time = 15
    
    #主相位数 principal phase position number
    p_p_p_num=3
    
    # 初始化系统旅行时间值
    TotalTravelTime = 0
    TravelTime = 0
    TravelTimeArray = [TravelTime]
    
    departed_number = []
    

    # Initial tp and T

    tp1 = 45

    tp2 = 15

    tp3 = 30

    ty12 = 3

    ty23 = 3

    ty31 = 3

    T = 99

    # Pre-trained for the simulation environment
    while step<600:
        
        flowp1 = 0

        flowp2 = 0

        flowp3 = 0

        for t in range(1,tp1+1):
            traci.trafficlight.setPhase('cluster_node2_node2-1',0)
            step += 1
            traci.simulationStep()
            flowp1 += get_nump1()
        for t in range(1,ty12+1):
            traci.trafficlight.setPhase('cluster_node2_node2-1',1)
            step += 1
            traci.simulationStep()
        for t in range(1,tp2+1):
            traci.trafficlight.setPhase('cluster_node2_node2-1',2)
            step += 1
            traci.simulationStep()
            flowp2 += get_nump2()
        for t in range(1,ty23+1):
            traci.trafficlight.setPhase('cluster_node2_node2-1',3)
            step += 1
            traci.simulationStep()
        for t in range(1,tp3+1):
            traci.trafficlight.setPhase('cluster_node2_node2-1',4)
            step += 1
            traci.simulationStep()
            flowp3 += get_nump3()
        for t in range(1,ty31+1):
            traci.trafficlight.setPhase('cluster_node2_node2-1',5)
            step += 1

        traci.simulationStep()
    
    # Calculate the first state of the learning process
    observation = CalcState(T)

    FLOW.append([flowp1,flowp2,flowp3])

    #判断是否堵
    index = 0

    # 训练从3601秒开始直至14400秒
    while step <= 3600:
        
        action=agent.choose_action(observation,i,number_of_simulation)

        Action.append(action)           # 记录动作序列
        
        T = T + 5*(action-1)

        TA.append(T)

        [last_flow1,last_flow2,last_flow3] = FLOW[-1]

        tp2 = int((T-9)*(last_flow2/(last_flow1+last_flow2+last_flow3)))

        if tp2 <= 15:

            tp2 = 15

        tp3 = int((T-9)*(last_flow3/(last_flow1+last_flow2+last_flow3)))

        if tp3 <= 15:

            tp3 = 15
        
        tp1 = int(T-9-tp2-tp3)

        P.append([tp1,tp2,tp3])

        index = 0                       # 道路系统运行卡死时的记录值（防止卡机）
        
        flowp1 = 0

        flowp2 = 0

        flowp3 = 0

        for t1 in range(1,tp1+1):
            
            departed_number.append(traci.simulation.getDepartedNumber())

            TravelTime = get_traveltime() 

            TravelTimeArray.append(TravelTime)

            TotalTravelTime += TravelTime

            flowp1 += get_nump1()

            traci.trafficlight.setPhase('cluster_node2_node2-1',0)

            step += 1

            traci.simulationStep()
    
        for t2 in range(1,ty12+1):

            departed_number.append(traci.simulation.getDepartedNumber())
            
            TravelTime = get_traveltime()   
            
            TravelTimeArray.append(TravelTime)

            TotalTravelTime += TravelTime

            traci.trafficlight.setPhase('cluster_node2_node2-1',1)

            step += 1
            
            traci.simulationStep()

        for t3 in range(1,tp2+1):

            departed_number.append(traci.simulation.getDepartedNumber())
            
            TravelTime = get_traveltime()  
            
            TravelTimeArray.append(TravelTime)

            TotalTravelTime += TravelTime

            flowp2 += get_nump2()

            traci.trafficlight.setPhase('cluster_node2_node2-1',2)
            
            step += 1
            
            traci.simulationStep()

        for t4 in range(1,ty23+1):

            departed_number.append(traci.simulation.getDepartedNumber())
            
            TravelTime = get_traveltime() 
            
            TravelTimeArray.append(TravelTime)

            TotalTravelTime += TravelTime

            traci.trafficlight.setPhase('cluster_node2_node2-1',3)
            
            step += 1
            
            traci.simulationStep()

        for t5 in range(1,tp3+1):
            traci.trafficlight.setPhase('cluster_node2_node2-1',4)

            TravelTime = get_traveltime()
            
            TravelTimeArray.append(TravelTime)

            TotalTravelTime += TravelTime

            flowp3 += get_nump3()

            traci.trafficlight.setPhase('cluster_node2_node2-1',4)
            
            step += 1
            
            traci.simulationStep()

        for t5 in range(1,ty31+1):

            departed_number.append(traci.simulation.getDepartedNumber())
           
            TravelTime = get_traveltime() 
           
            TravelTimeArray.append(TravelTime) 

            TotalTravelTime += TravelTime

            traci.trafficlight.setPhase('cluster_node2_node2-1',5)
                
            step += 1

            traci.simulationStep()

        if np.sum(departed_number[-50 :]) == 0:
           
            print('out')
           
            index = 1
           
            break

        # Calculate the first state of the learning process
        observation_ = CalcState(T)

        FLOW.append([flowp1,flowp2,flowp3])

        reward = calcreward(Reward_step)      # 本次动作的回报值 = 动作决策后时间内的累计瓶颈输出流量
 
        Reward.append(reward)       # 记录回报值序列

        # Store the transition matrix
        agent.learn(observation, action, reward, observation_, step)

        # Update the state for the next iteration
        observation = observation_

        iteration += 1

    traci.close()
    
    sys.stdout.flush()     

    return Reward, TravelTimeArray, TotalTravelTime, Action, index ,T ,P 


if __name__ == "__main__":
    options = get_options()    

    if options.nogui:
        sumoBinary = checkBinary('sumo')
    else:
        sumoBinary = checkBinary('sumo-gui')
    # number of action sets and features
    n_actions = 3
    n_features = 4
    number_of_simulation = 1000
    Game_Name = "Cross"
    TotalRewardSet = []
    RewardSet = []
    TotalTravelTime = 0
    TravelTime = []
    TravelTimeSet = []
    TotalTravelTimeSet = []
    ActionSet = []
    # this is the normal way of using traci. sumo is started as a
    # subprocess and then the python script connects and runs
    agent = QLearningTable(n_actions)

    i = 0

    while i <number_of_simulation:

        i = i + 1

        traci.start([sumoBinary, "-c", "simulation.sumocfg"])

        Reward,TravelTime,TotalTravelTime,Action,Index,T ,P = run(i, number_of_simulation)

        if Index == 0:

            RewardSet.append(Reward)

            TotalRewardSet.append(sum(Reward))

            TravelTimeSet.append(TravelTime)

            TotalTravelTimeSet.append(TotalTravelTime)

            ActionSet.append(Action)

            print(i)
            
            plt.plot(np.arange(len(TotalTravelTimeSet)), TotalTravelTimeSet)
            
            plt.show()

            plt.pause(0.1)

            plt.close()

        else:

            i = i - 1

