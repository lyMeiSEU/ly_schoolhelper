# -*- coding: utf-8 -*-
"""
Created on Wed May  8 19:26:32 2019

@author: lenovo
"""

import numpy as np

import pandas as pd





class QLearningTable:

    def __init__(self, n_actions, learning_rate=0.01, reward_decay=0.9, e_greedy_begin=0.1, e_greedy_end=0.9, explore_time= 5000000):

        self.actions = np.arange(n_actions)  # a list

        self.lr = learning_rate

        self.gamma = reward_decay

        self.epsilon=e_greedy_begin

        self.e_greedy_begin = e_greedy_begin
        
        self.e_greedy_end = e_greedy_end

        self.explore_time = explore_time

        self.q_table = pd.DataFrame(columns=self.actions, dtype=np.float64)



    def choose_action(self, observation,i, number_of_simulation):

        self.check_state_exist(observation)
            

        if self.epsilon < self.e_greedy_end:

            self.epsilon = self.epsilon + (self.e_greedy_end - self.e_greedy_begin) / self.explore_time
        
        
        
        #trick
#        n=10
         
        #interval=number_of_simulation/n
         
# =============================================================================
#          
#         if i<400:#i< ((n-5)*interval):
#                 if i>=0 and i<=1:
#                     epsilon=0
#                 else:
#                     epsilon=1
# #epsilon=self.epsilon
#         else:
#             epsilon=1
# =============================================================================
            
            
            
            # action selection

        if np.random.uniform() < self.epsilon:

            # choose best action

            state_action = self.q_table.loc[observation, :]

            # some actions may have the same value, randomly choose on in these actions

            action = np.random.choice(state_action[state_action == np.max(state_action)].index)

        else:

            # choose random action

            action = np.random.choice(self.actions)

        return action



    def learn(self, s, a, r, s_, step):
        
        self.check_state_exist(s)

        self.check_state_exist(s_)

        q_predict = self.q_table.loc[s, a]

        if step != 14400:

            q_target = r + self.gamma * self.q_table.loc[s_, :].max()  # next state is not terminal

        else:

            q_target = r  # next state is terminal

        if step>=3601 and step<=14400:
            
            lr=self.lr
            
        else:
            
            lr=0
        
        self.q_table.loc[s, a] += lr * (q_target - q_predict)  # update



    def check_state_exist(self, state):

        if state not in self.q_table.index :

            # append new state to q table

            self.q_table = self.q_table.append(

                pd.Series(

                    [0]*len(self.actions),

                    index=self.q_table.columns,

                    name=state

                )

            )

