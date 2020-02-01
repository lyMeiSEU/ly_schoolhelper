# -*- coding: utf-8 -*-
import pymysql
import json
import numpy as np
from LoginPost import checkSchoolServer
from systemMonitor import My_system
from globalManager import gol
from collections import defaultdict
from PIL import Image
from threading import Thread
import struct
import requests
import time
import hashlib
import base64
import socket
import types
import multiprocessing
import os
import datetime

serverUnit = ["49.235.241.216"]


class MysqlUtil(object):
    name = 'MysqlUtil'
    """
    use for connect Mysql ,need sql words
    """

    def __init__(self):
        try:
            # 创建链接
            self.conn = pymysql.connect(
                host='49.235.241.216',  # 连接你要取出数据库的ip，如果是本机可以不用写
                port=3306,  # 端口
                user='root',  # 你的数据库用户名
                passwd='Mly200211',  # 你的数据库密码
                db='basicdb',  # 数据库
                charset='utf8'
            )
            # 创建一个游标对象
            self.cur = self.conn.cursor(pymysql.cursors.DictCursor)
            # 服务器开启进行数据库维护
            sql_TableCreate = "create table if not exists Users(userNumber int, userType int, userAccount varchar(255), currentIngtion double, currentSilence double" +\
                ", userAllTime double, userAim text)"
            sql_TableDataMaintain = "DELETE FROM Users WHERE userAccount IN (SELECT userAccount FROM (" +\
                "SELECT userAccount FROM Users GROUP BY userAccount HAVING COUNT(userAccount)>1) e)" +\
                "AND userAllTime NOT IN (SELECT userAllTime FROM (SELECT MAX(userAllTime) AS userAllTime FROM Users GROUP BY userAccount HAVING COUNT(userAccount)>1) t)"
            sql_UserCheck = "select * from Users"
            self.conn.ping()
            cursor = self.conn.cursor()
            cursor.execute(sql_TableCreate)
            cursor.execute(sql_TableDataMaintain)
            cursor.execute(sql_UserCheck)
            worldList = []
            for Users in cursor:
                worldList.append([Users])
            print(worldList)
            self.conn.commit()
        except Exception as e:
            print("mysql error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()

    def mysqlAll(self, js, ran_str_cookie=""):
        try:
            global mysqlutil
            try:
                Js = json.loads(js)
            except Exception as e:
                Js = js
            # 解析客户端请求并执行相应操作
            if Js["method"] == "getServerInfo":  # 获取服务器运行状态
                return str(self.systemInfo())
            elif Js["method"] == 'getUserInfo':  # 获取用户信息
                return str(self.getAllUser())
            elif Js["method"] == "userLogin":  # 用户登陆，调用学校验证服务
                return str(self.Login(Js["data"][0], ran_str_cookie))
            elif Js["method"] == "userCreate":  # 用户登陆
                return str(self.Create(Js["data"][0]))
            elif Js["method"] == "userDelete":  # 新增用户表格
                return str(self.DeleteUser(Js["userAccount"]))
            elif Js["method"] == "userJobFinish":  # 新增用户记录
                return str(self.JobFinish(Js))
            elif Js["method"] == "userToDoUpdate":  # 维护用户ToDo列表
                return str(self.ToDoUpdata(Js))
            elif Js["method"] == "userAllTime" or Js["method"] == "currentIngtion":  # 计算用户燃度排行榜 计算用户学习总时间排行
                return str(self.LeardBoard(Js))
            elif Js["method"] == "getToDoList":  # 返回Todo列表
                return str(self.getToDoList(Js["userAccount"]))
            elif Js["method"] == "getUserTime":  # 返回用户学习时间
                return str(self.getUserTime(Js["userAccount"]))
            elif Js["method"] == "getAverageTime":  # 返回用户每日平均学习时间
                return str(self.AverageTime(Js))
            elif Js["method"] == "getIngtion":  # 返回用户燃度值
                return str(self.Ingtion(Js))
            elif Js["method"] == "getAllTime":  # 返回用户总学习时长
                return str(self.AllTime(Js["userAccount"]))
            elif Js["method"] == "userReportForm":  # 返回用户报表
                return str(self.reportForm(Js))
        except Exception as e:
            print("mysql error ", e)
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)
        finally:
            # 关闭游标和连接
            try:
                self.cur.close()
                self.conn.close()
            except Exception as e:
                mysqlutil = MysqlUtil()

    def reportForm(self, Js):
        try:
            Ingtion = self.Ingtion(Js)  # 用户燃度
            AllTime = self.AllTime(Js["userAccount"])  # 用户学习时长
            AverageTime = self.AverageTime(Js)  # 平均学习时间
            sql_selectDistinctJob = "select * from %sJob"\
                % (Js["userAccount"])
            self.conn.ping()
            cursor = self.conn.cursor()
            cursor.execute(sql_selectDistinctJob)
            Day = defaultdict(list)
            Time = 0
            for job in cursor:
                try:
                    startTime = datetime.datetime.strptime(
                        job[3], '%Y-%m-%d %H:%M:%S:%f')
                    endTime = datetime.datetime.strptime(
                        job[4], '%Y-%m-%d %H:%M:%S:%f')
                    Time = endTime-startTime  # 结束-开始计算任务时间
                except Exception as e:
                    continue
                if str(job[3][0:9]) not in Day:  # 这天还未计入
                    Day[str(job[3][0:10])] = Time
                else:
                    Day[str(job[3][0:10])] = Day[str(job[3][0:10])]+Time
            everyDay = ''
            num = 0
            for key in Day.keys():
                # 单独尝试获取datetime对象中的每个值
                try:
                    hour = Day[key].hours
                except:
                    hour = 0
                try:
                    minute = Day[key].minutes
                except:
                    minute = 0
                try:
                    seconds = Day[key].seconds
                except:
                    seconds = 0
                try:
                    microseconds = Day[key].microseconds
                except:
                    microseconds = 0
                All_hour = hour+minute/60+seconds/3600
                if num == 0:
                    everyDay = everyDay + \
                        '"%s":"%s"' % (str(num), str(All_hour))
                else:
                    everyDay = everyDay + \
                        ',"%s":"%s"' % (str(num), str(All_hour))
                num = num+1
            # 不足一周，补零补足
            if num < 7:
                while num < 7:
                    if num == 0:
                        everyDay = everyDay+'"%s":"%s"' % (str(num), str(0.0))
                    else:
                        everyDay = everyDay+',"%s":"%s"' % (str(num), str(0.0))
                    num = num+1
            result = '{"userIngtion":"%s","userAllTime":"%s","userAverageTime":"%s",%s}'\
                % (str(Ingtion), str(AllTime), str(AverageTime), str(everyDay))
            return str(result)
        except Exception as e:
            print("Report Form error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)

    def systemInfo(self):
        try:
            my_system = My_system()
            sys_data = defaultdict(list)
            finalData = '%s,%s,%s' % (str(my_system.get_disk_info()), str(
                my_system.get_cpu_info()), str(my_system.get_memory_info()))
            return finalData
        except Exception as e:
            print("System Info error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)

    def AllTime(self, userAccount):
        try:
            # 直接读Users，返回动态维护的AllTime列表
            sql_AllTime = 'select userAllTime from Users where userAccount="%s"' % (
                str(userAccount))
            print(sql_AllTime)
            self.conn.ping()
            cursor = self.conn.cursor()
            cursor.execute(sql_AllTime)
            return str(cursor.fetchone()[0])
        except Exception as e:
            print("User AllTime error "+str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)

    def Ingtion(self, Js):
        try:
            sql_Ingtion = 'select currentIngtion from Users where userAccount="%s"' % (
                str(Js["userAccount"]))
            print(sql_Ingtion)
            self.conn.ping()
            cursor = self.conn.cursor()
            cursor.execute(sql_Ingtion)
            return str(cursor.fetchone()[0])
        except Exception as e:
            print("User Ingtion error "+str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)

    def AverageTime(self, Js):
        try:
            # 读取Job列表，并计算每天平均学习时长
            sql_AverageTime = "select jobEndTime from %sJob" % (
                str(Js["userAccount"]))
            print(sql_AverageTime)
            self.conn.ping()
            cursor = self.conn.cursor()
            cursor.execute(sql_AverageTime)
            Date = ''
            num = 0
            for Day in cursor:
                if Date.find(str(Day[0])) == -1:
                    num = num+1
                    Date = Date+',%s' % (str(Day[0]))
            try:
                Average=float(self.AllTime(Js["userAccount"]))/num
            except:
                Average=0.0
            return str(Average)
        except Exception as e:
            print("AverageTime error "+str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)

    def DeleteUser(self, userAccount):
        try:
            # 供给web管理员管理用户使用，删除某个用户
            sql_DeleteUser = "delete from Users where userAccount='%s'" % (
                str(userAccount))
            sql_DeleteJobATodo = "drop table %sJob,%sTodo" % (
                str(userAccount), str(userAccount))
            print(sql_DeleteUser)
            print(sql_DeleteJobATodo)
            self.conn.ping()
            cursor = self.conn.cursor()
            cursor.execute(sql_DeleteUser)
            cursor.execute(sql_DeleteJobATodo)
            return "Ture"
        except Exception as e:
            print("Delete User error "+str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)

    def getAllUser(self):
        try:
            # 供给web管理员使用，获取总用户信息
            sql = "select * from Users"
            print(sql)
            self.conn.ping()
            cursor = self.conn.cursor()
            cursor.execute(sql)
            users = ''
            num = 0
            for User in cursor:
                if num == 0:
                    users = users+'{"userNumber":"%s","userType":"%s","userAccount":"%s","currentIngtion":%s,"currentSilence":%s}'\
                        % (str(User[0]), str(User[1]), str(User[2]), str(User[3]), str(User[4]))
                else:
                    users = users+',{"userNumber":"%s","userType":"%s","userAccount":"%s","currentIngtion":%s,"currentSilence":%s}'\
                        % (str(User[0]), str(User[1]), str(User[2]), str(User[3]), str(User[4]))
                num = num+1
            users = '{"num":%s,"data":[%s]}' % (str(num), str(users))
            return users
        except Exception as e:
            print("Get All User error "+str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)

    def Login(self, Js, ran_str_cookie):
        try:
            try:
                # 首先查找Users列表判断用户曾经是否登陆，无则验证用户并创建用户Job与Todo表
                sqlQ = "select * from Users where userAccount='%s'" % (
                    Js["userAccount"])
                print(sqlQ)
                self.conn.ping()
                cursor = self.conn.cursor()
                cursor.execute(sqlQ)
            except Exception as e:
                print("New user")
            worldList = []
            for Users in cursor:
                worldList.append([Users])
            schoolCheck = gol.get_schoolCheck(ran_str_cookie)
            if worldList == [] and schoolCheck.checkLogin(Js["userPassword"], Js["userAccount"], Js["verCode"]) == True:
                self.Create(Js)  # 用户未登陆过，创建用户表格
                return True
            return schoolCheck.checkLogin(Js["userPassword"], Js["userAccount"], Js["verCode"]) # 使用学校接口服务判断是否正确
        except Exception as e:
            print("Login error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)

    def Create(self, Js):
        try:
            # 查询用户是否存在
            sqlQ = "select * from Users where userAccount='%s'" % (
                Js["userAccount"])
            print(sqlQ)
            self.conn.ping()
            cursor = self.conn.cursor()
            cursor.execute(sqlQ)
            worldList = []
            for Users in cursor:
                worldList.append([Users])
            # 用户存在不允许再创建
            if worldList != []:
                return "User exist"
            sql_UserCheck = "select * from Users"
            cursor.execute(sql_UserCheck)
            num = 0
            for Users in cursor:
                num = num+1
            # 用户表格中插入用户信息
            if Js["userType"]=='normal':
                type=1
            else:
                type=0
            sql_UserAdd = "insert into Users(currentIngtion,currentSilence,userNumber,userAccount,userAllTime,userAim,userType) values(0,0,%s,'%s',0,'Aim',%s)" \
                % (str(num), str(Js["userAccount"]), str(type))
            print(sql_UserAdd)
            cursor.execute(sql_UserAdd)
            # 创建用户Job表格
            sql_UserJobTable = "create table if not exists %s" % (Js["userAccount"])+"Job(jobName varchar(255), " +\
                "jobSetDuration int, jobCreateTime varchar(255), jobStartTime varchar(255), jobEndTime varchar(255)," +\
                "jobType int, jobScene varchar(255), jobAlreadyTime double, concentration int, circularity int,jobSusbendTime double)"
            print(sql_UserJobTable)
            cursor.execute(sql_UserJobTable)
            # 创建用户Todo表格
            sql_UserToDoTable = "create table if not exists %s" % (Js["userAccount"])+"Todo(jobName varchar(255), " +\
                "jobSetDuration int, jobCreateTime varchar(255), jobStartTime varchar(255), jobEndTime varchar(255)," +\
                "jobType int, jobScene varchar(255), jobAlreadyTime double)"
            print(sql_UserToDoTable)
            cursor.execute(sql_UserToDoTable)
            self.conn.commit()
            return "True"
        except Exception as e:
            print("Create error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)

    def JobFinish(self, Js):
        try:
            self.conn.ping()
            cursor = self.conn.cursor()
            AllTime = 0.0
            # 任务完成，更新用户AllTime
            if Js["jobSetDuration"] == Js["jobAlreadyTime"]:
                sql_FindAllTime = "select userAllTime from Users where userAccount='%s'" \
                    % (Js["userAccount"])
                print(sql_FindAllTime)
                cursor.execute(sql_FindAllTime)
                AllTime = cursor.fetchone()[0]
                sql_AddAllTime = "update Users set userAllTime=%s where userAccount='%s'"\
                    % (str(AllTime+Js["jobSetDuration"]), Js["userAccount"])
                print(sql_AddAllTime)
                cursor.execute(sql_AddAllTime)
            # 增加用户记录
            sql_AddRecord = "insert into %s" % (Js["userAccount"])+"Job values('%s',%s,'%s','%s','%s',%s,'%s',%s,%s,%s,%s)"\
                % (str(Js["jobName"]), str(Js["jobSetDuration"]), str(Js["jobCreateTime"]), str(Js["jobStartTime"]), str(Js["jobEndTime"]), str(Js["jobType"]), str(Js["jobScene"]), str(Js["jobAlreadyTime"]), str(Js["concentration"]), str(Js["circularity"]), str(Js["jobSusbendTime"]))
            print(sql_AddRecord)
            cursor.execute(sql_AddRecord)
            self.conn.commit()
            return "Job Recode Add Success"
        except Exception as e:
            print("JobFinish error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)

    def getToDoList(self, userAccount):
        try:
            # 获取表中存储的ToDo任务
            sql_GetCaS = "select currentIngtion,currentSilence from Users where userAccount='%s'" % (
                userAccount)
            sql_TodoList = "select * from %s" % (userAccount)+"Todo"
            print(sql_GetCaS)
            print(sql_TodoList)
            self.conn.ping()
            cursor = self.conn.cursor()
            cursor.execute(sql_GetCaS)
            TodoList = '[{"currentIngtion":' + \
                str(cursor._rows[0][0])+',' + \
                '"currentSilence":'+str(cursor._rows[0][1])+'}'
            cursor.execute(sql_TodoList)
            num = 0
            for job in cursor:
                job_Object = ',{"jobName":"'+str(job[0])+'","jobSetDuration":'+str(job[1]) +\
                    ',"jobCreateTime":"'+str(job[2])+'","jobStartTime":"'+str(job[3])+'","jobEndTime":"'+str(job[4]) +\
                    '","jobType":'+str(job[5])+',"jobScene":"'+str(job[6]) +\
                    '","jobAlreadyTime":'+str(job[7])+'}'
                TodoList = TodoList+job_Object
            TodoList = TodoList+']'
            print(TodoList)
            # 返回ToDo列表给客户端
            return TodoList
        except Exception as e:
            print("getToDoList error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)

    def ToDoUpdata(self, Js):
        try:
            # ToDo表格与客户端动态同步
            sql_CoverUserToDo = "delete from %s" % (Js["userAccount"])+"Todo"
            print(sql_CoverUserToDo)
            self.conn.ping()
            cursor = self.conn.cursor()
            cursor.execute(sql_CoverUserToDo)
            max_lngtion = 0
            max_Slience = 0
            for ToDo in Js["data"]:
                sql_ToDoUpdata = "insert into %s" % (Js["userAccount"])+"Todo values('%s',%s,'%s','%s','%s',%s,'%s','%s')"\
                    % (str(ToDo["jobName"]), str(ToDo["jobSetDuration"]), str(ToDo["jobCreateTime"]), str(ToDo["jobStartTime"]), str(ToDo["jobEndTime"]), str(ToDo["jobType"]), str(ToDo["jobScene"]), str(ToDo["jobAlreadyTime"]))
                print(sql_ToDoUpdata)
                cursor.execute(sql_ToDoUpdata)
            if int(Js["currentIngtion"]) > max_lngtion:
                max_lngtion = int(Js["tolgnition"])
            if int(Js["currentSilence"]) > max_Slience:
                max_Slience = int(Js["currentSilence"])
            sql_AddTolgnition = "update Users set currentIngtion=%s,currentSilence=%s where userAccount=%s" % (
                str(max_lngtion), str(max_Slience), str(Js["userAccount"]))
            print(sql_AddTolgnition)
            cursor.execute(sql_AddTolgnition)
            self.conn.commit()
            return "True"
        except Exception as e:
            print("ToDoUpdata error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return "ToDoUpdata error "+str(e)

    def LeardBoard(self, Js):
        try:
            # 由用户名找到用户累计时间
            sql_SelectTime = "select %s from Users where userAccount='%s'" % (Js["method"],
                                                                              str(Js["userAccount"]))
            print(sql_SelectTime)
            self.conn.ping()
            cursor = self.conn.cursor()
            cursor.execute(sql_SelectTime)
            Value = cursor.fetchone()[0]
            # 将值比用户大的都挑选出来，计算得出用户排名
            sql_Select = "select %s from Users" % (
                Js["method"])
            print(sql_Select)
            self.conn.ping()
            cursor = self.conn.cursor()
            cursor.execute(sql_Select)
            # 初始排名设置为0（列表中将返回自己）
            num = 0
            all_List = '['
            Ten_List = ''
            try:
                for Users in cursor:
                    num = num+1
                    if num < 10:
                        # 返回排名前十的用户信息值
                        Ten_List = Ten_List+',{"userRank":%s,"%s":"%s"}'\
                            % (str(num), str(Js["method"]), str(Users[0]))
                    else:
                        if Users[0] < Value:
                            break
                all_List = all_List+'{"num":%s}' % (str(num))+Ten_List
                for where in range(num-4, num+4):
                    # 返回总学习时间在该用户周围的用户信息
                    if where <= 10:
                        break
                    all_List = all_List+',{"userRank":%s,"%s":"%s"}'\
                        % (str(num), str(Js["method"]), str(cursor._rows[where][0]))
                self.conn.commit()
            except Exception as e:
                print(all_List)
            finally:
                all_List = all_List+']'
                print(all_List)
                return all_List
        except Exception as e:
            print("LeaderBoard error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return "LeaderBoard error "+str(e)

    def getUserTime(self, userAccount):
        try:
            sql_getTime = "select jobEndTime from %sJob where jobEndTime=(select max(jobEndTime) from %sJob)" % (
                userAccount, userAccount)
            print(sql_getTime)
            self.conn.ping()
            cursor = self.conn.cursor()
            cursor.execute(sql_getTime)
            try:
                return cursor.fetchone()[0]
            except Exception as e:
                return "NULL"
        except Exception as e:
            print("getUserTime error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)


if __name__ == '__main__':
    mysqlutil = MysqlUtil()
    mysqlutil.systemInfo()
