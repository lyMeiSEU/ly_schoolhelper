# -*- coding: utf-8 -*-
from __future__ import absolute_import
from __future__ import print_function
from multiprocessing import Process, Pool
from http.server import HTTPServer, BaseHTTPRequestHandler
from collections import defaultdict
from threading import Thread
import numpy as np
import matplotlib.pyplot as plt
import optparse
import subprocess
import sumolib
import traci
import time
import re
import xml.dom.minidom
import multiprocessing
import os
import json
import string
import pymysql
import threading
import sys
import random
sys.path.append('../单点枚举_1.27/')
sys.path.append('../单点智能DQN1/')
sys.path.append('../人工交互_1.27/')

from simulation import SinglePointSimulation
from runner import Runner
from interaction import Interaction


class MysumoUtil():
    def mysumoAll(self,js):
        try:
            try:
                Js = json.loads(js)
            except Exception as e:
                Js = js
            # 解析客户端请求并执行相应操作 
            if Js["method"]=="SingleRoadSimulation":
                print(str(Js["data"]["min_greentimme"])+' '+str(Js["data"]["max_greentimme"])+' '+str(Js["data"]["interval"]))
                return str(self.SingleRoadSUMO(Js["data"]["min_greentimme"],Js["data"]["max_greentimme"],Js["data"]["interval"]))
            if Js["method"]=="RunnerSumo":
                print(str(Js["data"]["EpSteps"])+' '+str(Js["data"]["EpNum"]))
                return str(self.RunnerSumo(Js["data"]["EpSteps"],Js["data"]["EpNum"]))
            if Js["method"]=="InteractionSumo":
                print(str(Js["data"]["phase1_green"])+' '+str(Js["data"]["phase1_yellow"])+' '+str(Js["data"]["phase2_green"])+' '+str(Js["data"]["phase2_yellow"])+' '+str(Js["data"]["phase3_green"])+' '+str(Js["data"]["phase3_yellow"]))
                return str(self.InteractionSumo(Js["data"]["phase1_green"],Js["data"]["phase1_yellow"],Js["data"]["phase2_green"],Js["data"]["phase2_yellow"],Js["data"]["phase3_green"],Js["data"]["phase3_yellow"]))
            
        except Exception as e:
            mysumoutil = MysumoUtil()
            return str(e)
        finally:
            mysumoutil = MysumoUtil()

    def SingleRoadSUMO(self,min_greentimme,max_greentimme,interval):
        try:
            simulation=SinglePointSimulation()
            simulation._init_(min_greentimme,max_greentimme,interval)
            TenSecoundTimes=0
            maxmum = max_greentimme - min_greentimme
            n = round(maxmum/interval)
            for i in range(0,n):
                for j in range(0,n-i):
                    TenSecoundTimes=TenSecoundTimes+n-i-j
            OneSecoundTimes=interval*interval
            str='{"s1":%s,"s2":%s}'%(TenSecoundTimes,OneSecoundTimes)
            print(str)
            return str
        except Exception as e:
            mysumoutil = MysumoUtil()
            return str(e)
    
    def RunnerSumo(self,EpSteps,EpNum):
        try:
            runner=Runner()
            return runner._init_(EpSteps,EpNum)
        except Exception as e:
            mysumoutil = MysumoUtil()
            return str(e)
    
    def InteractionSumo(self,phase1_green,phase1_yellow,phase2_green,phase2_yellow,phase3_green,phase3_yellow):
        try:
            interaction=Interaction()
            return interaction._init_(phase1_green,phase1_yellow,phase2_green,phase2_yellow,phase3_green,phase3_yellow)
        except Exception as e:
            mysumoutil = MysumoUtil()
            return str(e)
    
if __name__ == '__main__':
    mysumoutil = MysumoUtil()
