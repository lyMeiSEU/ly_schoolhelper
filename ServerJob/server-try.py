import scapy.all as scapy
import scapy
import json
import urllib
import cgi
from collections import defaultdict
from http.server import HTTPServer, BaseHTTPRequestHandler

#服务端
class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        print("收到来自:"+self.client_address[0]+"的连接")
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        self.wfile.write(json.dumps('{"Hello":"hello"}').encode("utf-8"))
    def do_POST(self):
        print("收到来自:"+self.client_address[0]+"的连接")
        self.send_response(301)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(length).decode('utf-8')
        print(post_data)
        # You now have a dictionary of the post data
        self.wfile.write(json.dumps('{"Hello":"hello"}').encode("utf-8"))

if __name__ == "__main__":
    s=HTTPServer(("",8089),MyServer)  #多线程的TCP服务端，可以同时开启多个任务等着客户端来连，来一个请求就处理一个
    s.serve_forever()