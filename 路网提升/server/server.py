# -*- coding: utf-8 -*-
from globalManager import gol
from LoginPost import checkSchoolServer
from sqlUtil import MysqlUtil
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
sys.path.append('./*')

# 服务端

mysqlutil = MysqlUtil()
dic_cookieHandler = defaultdict(list)


class MyServer(BaseHTTPRequestHandler):
    # 从a-zA-Z0-9生成指定数量的随机字符做为cookie
    # 处理get请求

    def do_GET(self):
        global mysqlutil
        self.send_response(200)
        ran_str_cookie = ''.join(random.sample(
            string.ascii_letters + string.digits, 24))
        dic_cookieHandler[self.client_address[0]].append(ran_str_cookie)
        # 发送响应头部并附带cookie
        self.send_header('cookie: '+ran_str_cookie +
                         '\n'+'Content-type', 'application/json')
        self.end_headers()
        schookCheck = checkSchoolServer()
        gol.set_schoolCheck(schookCheck, ran_str_cookie)
        # 发送维护的验证码
        self.wfile.write(gol.get_schoolCheck(ran_str_cookie).valcode.content)

    # 处理post请求
    def do_POST(self):
        global mysqlutil
        self.send_response(200)
        self.send_header('Content-type', 'json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.end_headers()
        head = str(self.headers)
        # 寻找头部cookie并与本地匹配，验证成功才响应
        if head.find('Cookie') == -1:
            print("No cookie")
            #self.wfile.write(str("No cookie").encode('utf-8'))
            # return "No cookie"
        flag=True
        ran_str_cookie=''
        for key in dic_cookieHandler[self.client_address[0]]: # 在键值对中寻找对应的正确cookie
            ran_str_cookie = str(key)
            if head[head.find('Cookie')+8:head.find('Cookie')+32] == ran_str_cookie:
                print(ran_str_cookie)
                flag=False
                break
        if flag:
            print("Wrong cookie")
            # self.wfile.write(str("Wrong cookie").encode('utf-8'))
            # return "Wrong cookie"
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
            # 建立sql数据库对象
            global mysqlutil
            # 获取处理结果
            respond = mysqlutil.mysqlAll(Js, ran_str_cookie)
            print(respond)
            # 反馈结果
            self.wfile.write(str(respond).encode('utf-8'))
        except Exception as e:
            self.wfile.write("json error ".encode('utf-8'))
            print("json error "+str(e))
            mysqlutil = MysqlUtil()


if __name__ == '__main__':
    gol._init()
    # 多线程的TCP服务端，可以同时开启多个任务等着客户端来连，来一个请求就处理一个
    s = HTTPServer(("", 8089), MyServer)
    s.serve_forever()
