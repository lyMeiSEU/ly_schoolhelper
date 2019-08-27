import json
import string
import socketserver
import pymysql
import threading
import sys
sys.path.append('./*')
from collections import defaultdict
from http.server import HTTPServer, BaseHTTPRequestHandler
from sqlUtil import MysqlUtil

online=defaultdict(list)
#服务端
class MyServer(BaseHTTPRequestHandler):
    #处理get请求
    def do_GET(self):
        print("收到来自:"+self.client_address[0]+"的连接")
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
    
    #处理post请求
    def do_POST(self):
        print("收到来自:"+self.client_address[0]+"的连接")
        self.send_response(301)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        try:
            length = int(self.headers['Content-Length'])
            data = self.rfile.read(length).decode('utf-8')
            print(data)
            Js=json.loads(data)    
            print(Js)
            global mysqlutil
            mysqlutil = MysqlUtil()
            respond=mysqlutil.mysqlAll(Js)
            print(respond)
            self.wfile.write(respond.encode('utf-8'))
        except Exception as e:
            mysqlutil = MysqlUtil()
            self.wfile.write("json error".encode('utf-8'))
            print("server error "+str(e))

if __name__ == '__main__':
    mysqlutil = MysqlUtil()
    s=HTTPServer(("",8089),MyServer)  #多线程的TCP服务端，可以同时开启多个任务等着客户端来连，来一个请求就处理一个
    s.serve_forever()