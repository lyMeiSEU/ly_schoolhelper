# -*- coding: ISO-8859-1 -*-
"""
Created on Wed May  8 19:40:41 2019

@author: lenovo
"""

# coding=ISO-8859-1
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
import sumolib
import traci
import time
import re
import cv2

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

# Extract the image ·½°¸Èý²¿·Ö
def CalcState(Step):
    
    vm1=traci.edge.getLastStepMeanSpeed('1')+traci.edge.getLastStepMeanSpeed('4')
    vm2=traci.edge.getLastStepMeanSpeed('2')+traci.edge.getLastStepMeanSpeed('3')
    vm3=traci.edge.getLastStepMeanSpeed('10')+traci.edge.getLastStepMeanSpeed('8')

    LightStatus = traci.trafficlight.getPhase('cluster_node2_node2-1')         # ÔÑµÀ¿ØÖÆÐÅºÅµÆÉ«
    if vm1<10:
        vm1=0
    else:
        if vm1<30:
            vm1=1;
        else:
            vm1=2
    
    if vm2<10:
        vm2=0
    else:
        if vm2<30:
            vm2=1;
        else:
            vm2=2
    
    if vm3<10:
        vm3=0
    else:
        if vm3<30:
            vm3=1;
        else:
            vm3=2
        
    vm1=int(vm1)
    vm2=int(vm2)
    vm3=int(vm3)
    LightStatus=int(LightStatus)
    
    State = np.array([vm1,vm2,vm3,LightStatus])   # ËÄÎ¬×´Ì¬¼¯
    
    # [ÉÏÓÎÃÜ¶È¡¢½»Ö¯ÇøÃÜ¶È¡¢ÔÑµÀÃÜ¶È¡¢ÐÅºÅµÆÉ«]
    return State


def run(i,simulation_time):
    #traci.gui.setZoom("View #0",700)
    # initialize the parameters
    step = 0
    iteration = 0
    reward_step = 0
    Reward = []
    Reward_step = []
    Action = []
    
    
    realaction = 0
    
    #×îÐ¡ÂÌµÆÊ±¼ä
    min_green_time=6
    
    #×îÐ¡Ôö³¤Ê±¼äµ¥Î»
    min_increase=2
    
    #Ö÷ÏàÎ»Êý principal phase position number
    p_p_p_num=4
    
    # ³õÊ¼»¯ÏµÍ³ÂÃÐÐÊ±¼äÖµ
    TotalTravelTime = 0
    TravelTime = 0
    TravelTimeArray = [TravelTime]
    
    loaded = 0
    arrived = 0
    depart_number = []
    
    # Pre-trained for the simulation environment
    while step<3601:
        realaction = (realaction + 1) % p_p_p_num
        traci.trafficlight.setPhase('cluster_node2_node2-1', realaction*2 % (2*p_p_p_num))

        for t in range(1,min_green_time+1):
            loaded += traci.simulation.getLoadedNumber()
            arrived += traci.simulation.getArrivedNumber()
            depart_number.append(traci.simulation.getDepartedNumber())
            TravelTime =(traci.edge.getLastStepVehicleNumber('1')+
                         traci.edge.getLastStepVehicleNumber('2')+
                         traci.edge.getLastStepVehicleNumber('3')+
                         traci.edge.getLastStepVehicleNumber('4')+
                         traci.edge.getLastStepVehicleNumber('5')+
                         traci.edge.getLastStepVehicleNumber('6')+
                         traci.edge.getLastStepVehicleNumber('7')+
                         traci.edge.getLastStepVehicleNumber('8')+
                         traci.edge.getLastStepVehicleNumber('9')+
                         traci.edge.getLastStepVehicleNumber('10'))
            TravelTimeArray.append(TravelTime)
            TotalTravelTime += TravelTime
            
            reward_step = - (traci.edge.getLastStepVehicleNumber('1')+                     
                             traci.edge.getLastStepVehicleNumber('4')+
                             traci.edge.getLastStepVehicleNumber('6')+    
                             traci.edge.getLastStepVehicleNumber('8'))
            Reward_step.append(reward_step)
            
            
            traci.simulationStep()
            step += 1

    #path = StoreState(step+1)
    traci.simulationStep()
    step += 1
    #getTestPicArray(path)
        
    # Calculate the first state of the learning process
    observation = CalcState(step)

    index = 0

    # ÑµÁ·´Ó3601Ãë¿ªÊ¼Ö±ÖÁ14400Ãë
    while step < 14400:
        action=agent.choose_action(str(observation),i,simulation_time)
        realaction = (realaction +action) % p_p_p_num         # ¸ù¾Ýµ±Ç°×´Ì¬Ñ¡ÔñÒ»¸ö¶¯×÷
        traci.trafficlight.setPhase('cluster_node2_node2-1',realaction)    

        Action.append(action)           # ¼ÇÂ¼¶¯×÷ÐòÁÐ

        index = 0                       # µÀÂ·ÏµÍ³ÔËÐÐ¿¨ËÀÊ±µÄ¼ÇÂ¼Öµ£¨·ÀÖ¹¿¨»ú£©
        
        if action:
            traci.trafficlight.setPhase('cluster_node2_node2-1',(realaction*2-1)%( 2* p_p_p_num ))
            for t in range(1,min_increase+1):
                loaded += traci.simulation.getLoadedNumber()
                arrived += traci.simulation.getArrivedNumber()
                depart_number.append(traci.simulation.getDepartedNumber())
                TravelTime =(traci.edge.getLastStepVehicleNumber('1')+
                             traci.edge.getLastStepVehicleNumber('2')+
                             traci.edge.getLastStepVehicleNumber('3')+
                             traci.edge.getLastStepVehicleNumber('4')+
                             traci.edge.getLastStepVehicleNumber('5')+
                             traci.edge.getLastStepVehicleNumber('6')+
                             traci.edge.getLastStepVehicleNumber('7')+
                             traci.edge.getLastStepVehicleNumber('8')+
                             traci.edge.getLastStepVehicleNumber('9')+
                             traci.edge.getLastStepVehicleNumber('10'))
                TravelTimeArray.append(TravelTime)
                TotalTravelTime += TravelTime
                
                reward_step = - (traci.edge.getLastStepVehicleNumber('1')+                     
                                 traci.edge.getLastStepVehicleNumber('4')+
                                 traci.edge.getLastStepVehicleNumber('6')+
                                 traci.edge.getLastStepVehicleNumber('8')+
                                 traci.edge.getLastStepVehicleNumber('10'))
                Reward_step.append(reward_step)

                traci.simulationStep()
                step += 1

            traci.trafficlight.setPhase('cluster_node2_node2-1',realaction*2)   
                # ¸ù¾Ý´Ë¶¯×÷ÔËÐÐ5Ãë£¨¸ù¾Ý×Ô¼ºÉèÖÃµÄÐÅºÅ¿ØÖÆÖÜÆÚ¾ö¶¨ÔËÐÐÊ±¼ä£©
            for i in range(1,min_green_time+1):
                loaded += traci.simulation.getLoadedNumber()
                arrived += traci.simulation.getArrivedNumber()
                depart_number.append(traci.simulation.getDepartedNumber())
                TravelTime =(traci.edge.getLastStepVehicleNumber('1')+
                             traci.edge.getLastStepVehicleNumber('2')+
                             traci.edge.getLastStepVehicleNumber('3')+
                             traci.edge.getLastStepVehicleNumber('4')+
                             traci.edge.getLastStepVehicleNumber('5')+
                             traci.edge.getLastStepVehicleNumber('6')+
                             traci.edge.getLastStepVehicleNumber('7')+
                             traci.edge.getLastStepVehicleNumber('8')+
                             traci.edge.getLastStepVehicleNumber('9')+
                             traci.edge.getLastStepVehicleNumber('10'))            
                TravelTimeArray.append(TravelTime)
                
                reward_step = - (traci.edge.getLastStepVehicleNumber('1')+                     
                                 traci.edge.getLastStepVehicleNumber('4')+
                                 traci.edge.getLastStepVehicleNumber('6')+
                                 traci.edge.getLastStepVehicleNumber('8')+
                                 traci.edge.getLastStepVehicleNumber('10'))
                Reward_step.append(reward_step)
                
                traci.simulationStep()
                step += 1
                TotalTravelTime += TravelTime
        else:
            for i in range(1,min_increase+1):
                loaded += traci.simulation.getLoadedNumber()
                arrived += traci.simulation.getArrivedNumber()
                depart_number.append(traci.simulation.getDepartedNumber())
                TravelTime =(traci.edge.getLastStepVehicleNumber('1')+
                             traci.edge.getLastStepVehicleNumber('2')+
                             traci.edge.getLastStepVehicleNumber('3')+
                             traci.edge.getLastStepVehicleNumber('4')+
                             traci.edge.getLastStepVehicleNumber('5')+
                             traci.edge.getLastStepVehicleNumber('6')+
                             traci.edge.getLastStepVehicleNumber('7')+
                             traci.edge.getLastStepVehicleNumber('8')+
                             traci.edge.getLastStepVehicleNumber('9')+
                             traci.edge.getLastStepVehicleNumber('10'))         
                TravelTimeArray.append(TravelTime)
                
                reward_step = - (traci.edge.getLastStepVehicleNumber('1')+                     
                                 traci.edge.getLastStepVehicleNumber('4')+
                                 traci.edge.getLastStepVehicleNumber('6')+
                                 traci.edge.getLastStepVehicleNumber('8')+
                                 traci.edge.getLastStepVehicleNumber('10'))
                Reward_step.append(reward_step)
                
                traci.simulationStep()
                step += 1
                TotalTravelTime += TravelTime
            

        if np.sum(depart_number[-50 :]) == 0 :
            index = 1;  
            break; 

        # Calculate the first state of the learning process
        observation_ = CalcState(step)

        # Reward Calculation       
        # Light+Delay+WaitingTime+Teleport
        reward = np.sum(Reward_step[-min_green_time:])         # ±¾´Î¶¯×÷µÄ»Ø±¨Öµ = ¶¯×÷¾ö²ßºóÊ±¼äÄÚµÄÀÛ¼ÆÆ¿¾±Êä³öÁ÷Á¿

        Reward.append(reward)       # ¼ÇÂ¼»Ø±¨ÖµÐòÁÐ

        # Store the transition matrix
        agent.learn(str(observation), action, reward, str(observation_))

        # Update the state for the next iteration
        observation = observation_
        iteration += 1

    traci.close()
    sys.stdout.flush()     
    
    return Reward,TravelTimeArray,TotalTravelTime,Action,index


if __name__ == "__main__":
    options = get_options()    

    if options.nogui:
        sumoBinary = checkBinary('sumo')
    else:
        sumoBinary = checkBinary('sumo-gui')
    # number of action sets and features
    n_actions = 2; 
    n_features = 4; 
    simulation_time = 20;
    Game_Name = "Cross"
    RewardMatrix = [];
    EpisodeTotalTravelTime = 0;
    TravelTimeStep = [];
    TravelTimeMatrix = [];
    SystemTravelTime = [];
    SUMOTravelTime = [];
    ActionSet = []
    # this is the normal way of using traci. sumo is started as a
    # subprocess and then the python script connects and runs
    agent = QLearningTable(n_actions)

    for i in range(1,simulation_time+1):

        traci.start([sumoBinary, "-c", "simulation.sumocfg"])

        RewardStep,TravelTimeStep,EpisodeTotalTravelTime,LightStatus,Index = run(i,simulation_time)

        if Index == 0:

            RewardMatrix.append(RewardStep)

            TravelTimeMatrix.append(TravelTimeStep)

            SystemTravelTime.append(EpisodeTotalTravelTime)

            ActionSet.append(LightStatus)

            print(i)
        else:
            i = i-1

    # Travel time calculated by me
    SysTimePlot = np.array(SystemTravelTime)
    plt.plot(np.arange(len(SystemTravelTime)), SystemTravelTime)
    plt.ylabel('System Travel Time')
    plt.xlabel('Episodes')
    plt.show()
