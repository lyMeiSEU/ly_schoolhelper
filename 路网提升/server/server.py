# -*- coding: utf-8 -*-
from sumoUtil import MysumoUtil
from multiprocessing import Process, Pool
from http.server import HTTPServer, BaseHTTPRequestHandler
from collections import defaultdict
from threading import Thread
from collections import defaultdict
import multiprocessing
import os
import json
import string
import pymysql
import threading
import sys
import random
import time
sys.path.append('./*')

# 服务端
mysumoutil = MysumoUtil() 

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        global mysumoutil
        self.send_response(200)
        self.end_headers()

    # 处理post请求
    def do_POST(self):
        global mysumoutil
        self.send_response(200)
        self.send_header('Content-type', 'json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        head = str(self.headers)
        # 回复响应头部
        try:
            # 获取客户端传输数据
            length = int(self.headers['Content-Length'])
            data = self.rfile.read(length).decode('utf-8')
            try:
                Js = json.loads(data) # 尝试解析为json对象
            except:
                Js = data 
            print(Js)
            # 建立sumo数据库对象
            global mysumoutil,pool
            # 获取处理结果
            respond = mysumoutil.mysumoAll(Js)
            print(respond)
            # 反馈结果
            self.wfile.write(str(respond).encode('utf-8'))
        except Exception as e:
            self.wfile.write("json error ".encode('utf-8'))
            print("json error "+str(e))
        mysumoutil = MysumoUtil()        
    


if __name__ == '__main__':
    # 多线程的TCP服务端，可以同时开启多个任务等着客户端来连，来一个请求就处理一个
    s = HTTPServer(("", 8089), MyServer)
    s.serve_forever()
