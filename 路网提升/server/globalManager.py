# -*- coding: utf-8 -*-
import random
import string
from LoginPost import checkSchoolServer
from collections import defaultdict

# 全局变量管理对象，维护从学校获取的登陆二维码
_global_dict = defaultdict(list)

class gol:
    def _init():  # 初始化
        global _global_dict

    # 设置连接与cookie键值对维护登陆qrcode
    def set_schoolCheck(schoolCheck,cookie):
        global _global_dict
        _global_dict[str(cookie)]=schoolCheck

    # 通过cookie获取登陆qrcode
    def get_schoolCheck(cookie):
        global _global_dict
        return _global_dict[str(cookie)]