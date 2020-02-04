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

from simulation import SinglePointSimulation


class MysumoUtil():
    def mysumoAll(self,js):
        try:
            try:
                Js = json.loads(js)
            except Exception as e:
                Js = js
            # 解析客户端请求并执行相应操作 
            if Js["method"]=="SingleRoadSimulation":
                return str(self.SingleRoadSUMO())
        except Exception as e:
            mysumoutil = MysumoUtil()
            return str(e)
        finally:
            mysumoutil = MysumoUtil()

    def SingleRoadSUMO(self):
        try:
            simulation=SinglePointSimulation()
            return simulation._init_(15,95,10)
        except Exception as e:
            mysumoutil = MysumoUtil()
            return str(e)

    
if __name__ == '__main__':
    mysumoutil = MysumoUtil()
