"""

@author: zemian
"""

# coding=utf-8
from __future__ import absolute_import
from __future__ import print_function

# from Q_table import QLearningTable
from DDQN_with_Prioritized_Replay import DQNPrioritizedReplay
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import os
import sys
import optparse
#import tensorflow as tf
import tensorflow.compat.v1 as tf
tf.disable_v2_behavior()

import traci

if 'SUMO_HOME' in os.environ:
    tools = os.path.join(os.environ['SUMO_HOME'], 'tools')
    sys.path.append(tools)
else:
    sys.exit("please declare environment variable 'SUMO_HOME'")


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


# we need to import python modules from the $SUMO_HOME/tools directory
# try:
#     sys.path.append(os.path.join(os.path.dirname(
#         __file__), '..', '..', '..', '..', "tools"))  # tutorial in tests
#     sys.path.append(os.path.join(os.environ.get("SUMO_HOME", os.path.join(
#         os.path.dirname(__file__), "..", "..", "..")), "tools"))  # tutorial in docs
#     from sumolib import checkBinary  # noqa
# except ImportError:
#     sys.exit(
#         "please declare environment variable 'SUMO_HOME' as the root directory of your sumo installation (it should contain folders 'bin', 'tools' and 'docs')")
#
# sumoBinary = "D:/documents/research/Simulation/SUMO/sumo-1.0.1/bin"
# sumoCmd = [sumoBinary, "-c", "simulation.sumocfg"]

def get_traveltime():  ####await correcting
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


def get_thru(last_N, last_E, last_S, last_W):
    N = (traci.inductionloop.getLastStepVehicleIDs('D6_1_0') +
         traci.inductionloop.getLastStepVehicleIDs('D6_1_1') +
         traci.inductionloop.getLastStepVehicleIDs('D6_1_2') +
         traci.inductionloop.getLastStepVehicleIDs('D6_1_3'))
    E = (traci.inductionloop.getLastStepVehicleIDs('D4_1_0') +
        traci.inductionloop.getLastStepVehicleIDs('D4_1_1'))
    S = (traci.inductionloop.getLastStepVehicleIDs('D8_1_0') +
         traci.inductionloop.getLastStepVehicleIDs('D8_1_1') +
         traci.inductionloop.getLastStepVehicleIDs('D8_1_2'))
    W = (traci.inductionloop.getLastStepVehicleIDs('D1_1_0'))

    N_dif = list(set(N).difference(set(last_N)))
    thru_N = len(N_dif)
    E_dif = list(set(E).difference(set(last_E)))
    thru_E = len(E_dif)
    S_dif = list(set(S).difference(set(last_S)))
    thru_S = len(S_dif)
    W_dif = list(set(W).difference(set(last_W)))
    thru_W = len(W_dif)
    thru_all = thru_N+thru_E+ thru_S+ thru_W

    return N, E, S, W, thru_all

def CalcState():
    # s1 = (traci.lane.getLastStepVehicleNumber('6_0') + traci.lane.getLastStepVehicleNumber('6_1')
    #       + traci.lane.getLastStepVehicleNumber('6_2'))   #phase 1, north conjunction, straight and right
    # s3 = (traci.lane.getLastStepVehicleNumber('6_0') + traci.lane.getLastStepVehicleNumber(
    #     '6_3'))  # phase 3, north conjunction, left and right

    s1 = (traci.lane.getLastStepVehicleNumber('6_0') + traci.lane.getLastStepVehicleNumber('6_1')
          + traci.lane.getLastStepVehicleNumber('6_2')+ traci.lane.getLastStepVehicleNumber('15_0')
          + traci.lane.getLastStepVehicleNumber('15_1')+ traci.lane.getLastStepVehicleNumber('15_2'))   #phase 1, north conjunction. straight, right and demand
    s2 = (traci.lane.getLastStepVehicleNumber('8_0') + traci.lane.getLastStepVehicleNumber('8_1'))  #phase 1, south conjunction, straight and right
    s3 = (traci.lane.getLastStepVehicleNumber('6_0') + traci.lane.getLastStepVehicleNumber('6_3')
          + traci.lane.getLastStepVehicleNumber('15_0') + traci.lane.getLastStepVehicleNumber('15_1')
          + traci.lane.getLastStepVehicleNumber('15_2'))  #phase 3, north conjunction. left, right and demand
    s4 = (traci.lane.getLastStepVehicleNumber('8_0') + traci.lane.getLastStepVehicleNumber('8_2'))  #phase 3, south conjunction, left and right
    s5 = (traci.lane.getLastStepVehicleNumber('1_0') )  #phase 5, west conjunction, straight, left and right
    s6 = (traci.lane.getLastStepVehicleNumber('4_0') + traci.lane.getLastStepVehicleNumber('4_1')) #phase 5, east conjunction, straight, left and right
    s7 = traci.trafficlight.getPhase('cluster_node2_node2-1')

    ######standarization
    s1 = s1/172.0
    s2 = s2/70.0
    s3 = s3/71.0
    s4 = s4/70.0
    s5 = s5/52.0
    s6 = s6/112.0
    s7 = s7/5.0

    state = np.array([s1, s2, s3, s4, s5, s6, s7])
    return state

def run(RL, number_of_simulation, ep_steps):
    phasetime_in = []
    phasenum_in = []

    reward_list = []
    traveltimeList = []
    for ep in range(ep_steps):
        options = get_options()
        if options.nogui:
            sumoBinary = checkBinary('sumo')
        else:
            sumoBinary = checkBinary('sumo')
        traci.start([sumoBinary, "-c", "simulation.sumocfg"])

        time_step = 0
        traveltime = 0
        # Pre-trained for the simulation environment
        while time_step < 600:
            for t in range(45):
                traci.trafficlight.setPhase('cluster_node2_node2-1', 0)
                time_step += 1
                traci.simulationStep()
                traveltime += get_traveltime()
            for t in range(3):
                traci.trafficlight.setPhase('cluster_node2_node2-1', 1)
                time_step += 1
                traci.simulationStep()
                traveltime += get_traveltime()
            for t in range(15):
                traci.trafficlight.setPhase('cluster_node2_node2-1', 2)
                time_step += 1
                traci.simulationStep()
                traveltime += get_traveltime()
            for t in range(3):
                traci.trafficlight.setPhase('cluster_node2_node2-1', 3)
                time_step += 1
                traci.simulationStep()
                traveltime += get_traveltime()
            for t in range(30):
                traci.trafficlight.setPhase('cluster_node2_node2-1', 4)
                time_step += 1
                traci.simulationStep()
                traveltime += get_traveltime()
            for t in range(3):
                traci.trafficlight.setPhase('cluster_node2_node2-1', 5)
                time_step += 1
                traci.simulationStep()
                traveltime += get_traveltime()

        last_phase = traci.trafficlight.getPhase('cluster_node2_node2-1')
        phase_time = 4
        yello_time = 3
        ep_reward = 0

        observation = CalcState()
        last_N, last_E, last_S, last_W = [], [], [],[]
        while time_step <= number_of_simulation:
            action = RL.choose_action(observation)  # action set: {0th phase,  2th phase , 4th phase}
            phase = action * 2

            # judge whether present phase is too large
            if phase == last_phase:
                if phase_time < 80:
                    extended_time = 4
                    phase_time += extended_time

                else:  ##phase time is larger than maximum phase time, switch to next phase
                    phase = (phase+2)%6
                    phase_time = 4

            if phase == last_phase:  # extend present phase
                thru_vehs = 0
                for t in range(extended_time):
                    traci.trafficlight.setPhase('cluster_node2_node2-1', phase)
                    time_step += 1
                    traci.simulationStep()
                    traveltime += get_traveltime()
                    last_N, last_E, last_S, last_W, thru_all = get_thru(last_N, last_E, last_S, last_W)
                    thru_vehs += thru_all

                thru_rate = thru_vehs/float(extended_time)
                reward = thru_rate
            else:  # switch to new phase
                thru_vehs = 0
                for t in range(yello_time):  # yellow phase
                    traci.trafficlight.setPhase('cluster_node2_node2-1', (last_phase + 1)%6)
                    time_step += 1
                    traci.simulationStep()
                    traveltime += get_traveltime()
                    last_N, last_E, last_S, last_W, thru_all = get_thru(last_N, last_E, last_S, last_W)
                    thru_vehs += thru_all
                for t in range(phase_time):
                    traci.trafficlight.setPhase('cluster_node2_node2-1', phase)
                    time_step += 1
                    traci.simulationStep()
                    traveltime += get_traveltime()
                    last_N, last_E, last_S, last_W, thru_all = get_thru(last_N, last_E, last_S, last_W)
                    thru_vehs += thru_all

                thru_rate = thru_vehs/float(yello_time + phase_time)
                reward = thru_rate


            observation_ = CalcState()
            ep_reward += thru_vehs
            RL.store_transition(observation, action, reward, observation_)  #reward is the reward divided by time
            RL.learn()
            observation = observation_


        ###end training
        traci.close()
        sys.stdout.flush()
        reward_list.append(ep_reward)
        traveltime = traveltime/3600
        traveltimeList.append(traveltime)
        print(str(ep)+"th ep_reward:"+str(ep_reward)+";travel time:"+str(traveltime))

    return reward_list, traveltimeList



if __name__ == "__main__":
    EpSteps = 3600  #in each episode, there are 150~750 steps
    EpNum = 20

    sess = tf.Session()
    with tf.variable_scope('Double_DQN'):
        double_DQN = DQNPrioritizedReplay(
            n_actions=3, n_features=7, memory_size=3000,
            e_greedy_increment=0.0002, prioritized=True, double_q=True, sess=sess)  #annealing steps: 0.98/e_increment
    sess.run(tf.global_variables_initializer())

    reward_list , traveltime_list= run(double_DQN, EpSteps, EpNum)


    # print(reward_list)
    # print(traveltime_list)
    plt.subplot(2, 1, 1)
    plt.plot(reward_list, '-', lw=2)
    plt.xlabel('episodes')
    plt.ylabel('reward')
    plt.grid(True)

    plt.subplot(2, 1, 2)
    plt.plot(traveltime_list, '-', lw=2)
    plt.xlabel('episodes')
    plt.ylabel('veh*h')
    plt.grid(True)
    #plt.show()
    plt.savefig('output.png')
    plt.close()




