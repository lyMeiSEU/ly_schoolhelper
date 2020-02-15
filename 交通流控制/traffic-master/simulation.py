# -*- coding: utf-8 -*-
"""
Created on Tue May 14 20:33:45 2019

@author: DD
"""

# coding=utf-8
from __future__ import absolute_import
from __future__ import print_function

from Q_table import QLearningTable
from sumolib import checkBinary
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
except Exception as e:
    sys.exit(
        "please declare environment variable 'SUMO_HOME' as the root directory of your sumo installation (it should contain folders 'bin', 'tools' and 'docs')")


def get_options():
    optParser = optparse.OptionParser()
    optParser.add_option("--nogui", action="store_true",
                         default=False, help="run the commandline version of sumo")
    options, args = optParser.parse_args()
    return options


def get_traveltime():
    TravelTime = (traci.edge.getLastStepVehicleNumber('1') +
                  traci.edge.getLastStepVehicleNumber('2') +
                  traci.edge.getLastStepVehicleNumber('3') +
                  traci.edge.getLastStepVehicleNumber('4') +
                  traci.edge.getLastStepVehicleNumber('5') +
                  traci.edge.getLastStepVehicleNumber('6') +
                  traci.edge.getLastStepVehicleNumber('7') +
                  traci.edge.getLastStepVehicleNumber('8') +
                  traci.edge.getLastStepVehicleNumber('9') +
                  traci.edge.getLastStepVehicleNumber('10') +
                  traci.edge.getLastStepVehicleNumber('11') +
                  traci.edge.getLastStepVehicleNumber('15'))
    return TravelTime


def get_flow1():
    flow1 = (traci.inductionloop.getLastStepVehicleNumber('D6_1_0') +
             traci.inductionloop.getLastStepVehicleNumber('D6_1_1') +
             traci.inductionloop.getLastStepVehicleNumber('D6_1_2') +
             traci.inductionloop.getLastStepVehicleNumber('D6_1_3'))
    return flow1


def get_flow2():
    flow2 = (traci.inductionloop.getLastStepVehicleNumber('D4_1_0') +
             traci.inductionloop.getLastStepVehicleNumber('D4_1_1'))
    return flow2


def get_flow3():
    flow3 = (traci.inductionloop.getLastStepVehicleNumber('D8_1_0') +
             traci.inductionloop.getLastStepVehicleNumber('D8_1_1') +
             traci.inductionloop.getLastStepVehicleNumber('D8_1_2'))
    return flow3


def get_flow4():
    flow4 = (traci.inductionloop.getLastStepVehicleNumber('D1_1_0'))
    return flow4


def get_flow():
    flow = get_flow1() + get_flow2() + get_flow3() + get_flow4()
    return flow


def calcreward_step():
    reward_step = get_flow()
    return reward_step


def calcreward(Reward_step):
    reward = np.sum(Reward_step[-3:])
    return reward


# Extract the image 方案三部分
def CalcState():
    s1 = (traci.lane.getLastStepVehicleNumber('15_0') + traci.lane.getLastStepVehicleNumber(
        '15_1') + traci.lane.getLastStepVehicleNumber('15_2') +
          traci.lane.getLastStepVehicleNumber('6_0') + traci.lane.getLastStepVehicleNumber('6_6') +
          traci.lane.getLastStepVehicleNumber('8_0') + traci.lane.getLastStepVehicleNumber('8_4'))  # 南北左右
    s2 = (traci.lane.getLastStepVehicleNumber('15_0') + traci.lane.getLastStepVehicleNumber(
        '15_1') + traci.lane.getLastStepVehicleNumber('15_2')  # 南北直右
          + traci.lane.getLastStepVehicleNumber('8_0') + traci.lane.getLastStepVehicleNumber('8_2')
          + traci.lane.getLastStepVehicleNumber('6_0') + traci.lane.getLastStepVehicleNumber(
                '6_2') + traci.lane.getLastStepVehicleNumber('6_4'))
    s3 = (traci.lane.getLastStepVehicleNumber('1_0')  # 东西直左右
          + traci.lane.getLastStepVehicleNumber('4_0')
          + traci.lane.getLastStepVehicleNumber('4_1'))

    LightStatus = traci.trafficlight.getPhase('cluster_node2_node2-1')  # 匝道控制信号灯色

    LightStatus = int(LightStatus)

    State = np.array([s1, s2, s3, LightStatus])  # n维状态集

    # [上游密度、交织区密度、匝道密度、信号灯色]
    return State


def a_step(departed, departed_number, TravelTimeArray, TotalTravelTime, Reward_step):
    traci.simulationStep()

    departed += traci.simulation.getDepartedNumber()

    departed_number.append(traci.simulation.getDepartedNumber())

    TravelTime = get_traveltime()
    # print(departed-arrived,TravelTime,departed-arrived==TravelTime)
    TravelTimeArray.append(TravelTime)

    TotalTravelTime += TravelTime

    Reward_step.append(calcreward_step())

    print(traci.trafficlight.getPhase('cluster_node2_node2-1'))

    return departed, departed_number, TravelTimeArray, TotalTravelTime, Reward_step


def run(i, number_of_simulation):
    # traci.gui.setZoom("View #0",700)
    # initialize the parameters
    global agent
    step = 0
    iteration = 0
    Reward_step = []
    Reward = []
    Action = []
    RealAction = []

    realaction = 0

    # 最小绿灯时间
    min_green_time = 3

    # 最小增长时间单位
    min_increase = 3

    # 主相位数 principal phase position number
    p_p_p_num = 3

    # 初始化系统旅行时间值
    TotalTravelTime = 0
    TravelTime = 0
    TravelTimeArray = [TravelTime]

    departed = 0
    departed_number = []

    # Pre-trained for the simulation environment
    while step < 3600:

        realaction = (realaction + 1) % p_p_p_num

        traci.trafficlight.setPhase('cluster_node2_node2-1', (realaction * 2 - 1) % (2 * p_p_p_num))

        # 黄灯
        for t in range(1, 4):
            departed, departed_number, TravelTimeArray, TotalTravelTime, Reward_step = a_step(departed, departed_number,
                                                                                              TravelTimeArray,
                                                                                              TotalTravelTime,
                                                                                              Reward_step)

            step += 1

        traci.trafficlight.setPhase('cluster_node2_node2-1', realaction * 2 % (2 * p_p_p_num))

        for t in range(1, min_green_time + 1):
            departed, departed_number, TravelTimeArray, TotalTravelTime, Reward_step = a_step(departed, departed_number,
                                                                                              TravelTimeArray,
                                                                                              TotalTravelTime,
                                                                                              Reward_step)

            step += 1

    # Calculate the first state of the learning process
    observation = CalcState()
    # 判断是否堵
    index = 0

    # 训练从3601秒开始直至14400秒
    while step <= number_of_simulation:

        action = agent.choose_action(str(observation), i, number_of_simulation)
        Action.append(action)  # 记录动作序列

        realaction = (realaction + action) % p_p_p_num  # 记录真实相位
        RealAction.append(realaction)

        index = 0  # 道路系统运行卡死时的记录值（防止卡机）

        if action:

            traci.trafficlight.setPhase('cluster_node2_node2-1', (realaction * 2 - 1) % (2 * p_p_p_num))
            # 黄灯
            for t in range(1, 4):
                departed, departed_number, TravelTimeArray, TotalTravelTime, Reward_step = a_step(departed,
                                                                                                  departed_number,
                                                                                                  TravelTimeArray,
                                                                                                  TotalTravelTime,
                                                                                                  Reward_step)

                step += 1

            traci.trafficlight.setPhase('cluster_node2_node2-1', realaction * 2 % (2 * p_p_p_num))
            # 根据此动作运行min_green_time秒（根据自己设置的信号控制周期决定运行时间）
            for i in range(1, min_green_time + 1):
                departed, departed_number, TravelTimeArray, TotalTravelTime, Reward_step = a_step(departed,
                                                                                                  departed_number,
                                                                                                  TravelTimeArray,
                                                                                                  TotalTravelTime,
                                                                                                  Reward_step)

                step += 1

        else:

            traci.trafficlight.setPhase('cluster_node2_node2-1', realaction * 2 % (2 * p_p_p_num))

            for i in range(1, min_green_time + 1):
                departed, departed_number, TravelTimeArray, TotalTravelTime, Reward_step = a_step(departed,
                                                                                                  departed_number,
                                                                                                  TravelTimeArray,
                                                                                                  TotalTravelTime,
                                                                                                  Reward_step)

                step += 1

        if np.sum(departed_number[-50:]) == 0:
            print('out')
            index = 1
            break

        # Calculate the first state of the learning process
        observation_ = CalcState()

        reward = calcreward(Reward_step)  # 本次动作的回报值 = 动作决策后时间内的累计瓶颈输出流量

        Reward.append(reward)  # 记录回报值序列

        # Store the transition matrix
        agent.learn(str(observation), action, reward, str(observation_), step)

        # Update the state for the next iteration
        observation = observation_

        iteration += 1

    traci.close()

    sys.stdout.flush()

    return Reward, TravelTimeArray, TotalTravelTime, Action, RealAction, index


# if __name__ == "__main__":
def simulation(num):

    options = get_options()

    if options.nogui:
        sumoBinary = checkBinary('sumo')
    else:
        sumoBinary = checkBinary('sumo-gui')
    # number of action sets and features
    n_actions = 2
    n_features = 4
    number_of_simulation = num
    Game_Name = "Cross"
    TOTALREWARD = []
    RewardMatrix = []
    EpisodeTotalTravelTime = 0
    TravelTimeStep = []
    TravelTimeMatrix = []
    SystemTravelTime = []
    SUMOTravelTime = []
    LightSet = []
    ActionSet = []
    # this is the normal way of using traci. sumo is started as a
    # subprocess and then the python script connects and runs
    global agent
    agent = QLearningTable(n_actions)

    for i in range(1, number_of_simulation + 1):

        traci.start([sumoBinary, "-c", "simulation.sumocfg"])

        RewardStep, TravelTimeStep, EpisodeTotalTravelTime, Action, LightStatus, Index = run(i, number_of_simulation)

        if Index == 0:

            RewardMatrix.append(RewardStep)

            TOTALREWARD.append(sum(RewardStep))

            TravelTimeMatrix.append(TravelTimeStep)

            SystemTravelTime.append(EpisodeTotalTravelTime)

            ActionSet.append(Action)

            LightSet.append(LightStatus)

            print(i)

            plt.plot(np.arange(len(SystemTravelTime)), SystemTravelTime)

            plt.show();

        else:
            i = i - 1
