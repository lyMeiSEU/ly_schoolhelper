# -*- coding: ISO-8859-1 -*-
"""
Created on Wed May  8 19:26:32 2019

@author: lenovo
"""

import numpy as np

import pandas as pd





class QLearningTable:

    def __init__(self, n_actions, learning_rate=0.0001, reward_decay=0.9, e_greedy_begin=0.6, e_greedy_end=0.99):

        self.actions = np.arange(n_actions)  # a list

        self.e_greedy_begin=e_greedy_begin
        
        self.e_greedy_end=e_greedy_end

        self.lr = learning_rate

        self.gamma = reward_decay

        self.q_table = pd.DataFrame(columns=self.actions, dtype=np.float64)



    def choose_action(self, observation, iteration, simulation_time):

        self.check_state_exist(observation)
        
        epsilon= self.e_greedy_begin+(self.e_greedy_end-self.e_greedy_begin)*iteration/simulation_time
        
        # action selection

        if np.random.uniform() < epsilon:

            # choose best action

            state_action = self.q_table.loc[observation, :]

            # some actions may have the same value, randomly choose on in these actions

            action = np.random.choice(state_action[state_action == np.max(state_action)].index)

        else:

            # choose random action

            action = np.random.choice(self.actions)

        return action



    def learn(self, s, a, r, s_):
        
        self.check_state_exist(s)

        self.check_state_exist(s_)

        q_predict = self.q_table.loc[s, a]

        if s_ != 'terminal':

            q_target = r + self.gamma * self.q_table.loc[s_, :].max()  # next state is not terminal

        else:

            q_target = r  # next state is terminal

        self.q_table.loc[s, a] += self.lr * (q_target - q_predict)  # update



    def check_state_exist(self, state):

        if str(state) not in self.q_table.index :

            # append new state to q table

            self.q_table = self.q_table.append(

                pd.Series(

                    [0]*len(self.actions),

                    index=self.q_table.columns,

                    name=str(state)

                )

            )

