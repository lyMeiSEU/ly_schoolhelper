import pymysql
import json


class MysqlUtil(object):
    name = 'MysqlUtil'
    """
    use for connect Mysql ,need sql words
    """

    def __init__(self):
        try:
            # 创建链接
            self.conn = pymysql.connect(
                host='139.180.168.111',  # 连接你要取出数据库的ip，如果是本机可以不用写
                port=3306,  # 端口
                user='root',  # 你的数据库用户名
                passwd='Mly200211',  # 你的数据库密码
                db='basicdb',  # 数据库
                charset='utf8'
            )
            # 创建一个游标对象
            self.cur = self.conn.cursor(pymysql.cursors.DictCursor)
            sql_TableCreate = "create table if not exists Users(userNumber int, userType int, userAccount char(20), userPassword char(20)" +\
                ", userAllTime double, userAim text)"
            sql_UserCheck = "select * from Users"
            cursor = self.conn.cursor()
            cursor.execute(sql_TableCreate)
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

    def mysqlAll(self, js):
        try:
            global mysqlutil
            Js = json.loads(js)
            if Js["method"] == "userLogin":
                return self.Login(Js["data"][0])
            elif Js["method"] == "userCreate":
                return self.Create(Js["data"][0])
            elif Js["method"] == "userJobFinish":
                return self.JobFinish(Js)
            elif Js["method"] == "userToDoUpdate":
                return self.ToDoUpdata(Js)
            elif Js["method"] == "userTolngitionLeaderBoard":
                return self.TolngitionLeardBoard(Js)
            elif Js["method"] == "userAllTimeLeaderBoard":
                return self.AllTimeLeaderBoard(Js)

        except Exception as e:
            print("mysql error ", e)
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)
        finally:
            # 关闭游标和连接
            self.cur.close()
            self.conn.close()
            return "True"

    def Login(self, Js):
        try:
            sql_PasswordSearch = "select userPassword from Users where userAccount='" + \
                Js["userAccount"]+"'"
            print(sql_PasswordSearch)
            cursor = self.conn.cursor()
            cursor.execute(sql_PasswordSearch)
            if Js["userPassword"] == cursor.fetchone()[0]:
                return "True"
            else:
                return "False"
        except Exception as e:
            print("Login error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)

    def Create(self, Js):
        try:
            sqlQ = "select * from Users where userAccount='" + \
                Js["userAccount"]+"'"
            print(sqlQ)
            cursor = self.conn.cursor()
            cursor.execute(sqlQ)
            worldList = []
            for Users in cursor:
                worldList.append([Users])
            if worldList != []:
                return "User exist"
            sql = "insert into Users(userType,userAccount,userPassword) values("+str(Js["userType"]) +\
                ","+"'"+Js["userAccount"]+"'"+","+"'"+Js["userPassword"]+"');"
            print(sql)
            cursor = self.conn.cursor()
            cursor.execute(sql)
            self.conn.commit()
            sql_UserJobTable = "create table if not exists "+Js["userAccount"]+"Job(jobName String, " +\
                "jobSetDuration double, jobCreateTime datetime, jobStartTime datetime, jobEndTime datetime, jobSusbendTime datetime, "\
                "jobType int, jobScene String, jobAlreadyTime double, Concentration int, Circularity int);"
            print(sql_UserJobTable)
            cursor = self.conn.cursor()
            cursor.execute(sql_UserJobTable)
            self.conn.commit()
            sql_UserToDoTable = "create table if not exists "+Js["userAccount"]+"Todo(jobName String, " +\
                "jobSetDuration double, jobCreateTime datetime, jobStartTime datetime, jobEndTime datetime, jobSusbendTime datetime, "\
                "jobType int, jobScene String, jobAlreadyTime double, currentTime datetime, currentlngtion double, currentSlience double, tolgnition double);"
            print(sql_UserToDoTable)
            cursor = self.conn.cursor()
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
            cursor = self.conn.cursor()
            AllTime=0.0
            if Js["jobSetDuration"] == Js["jobAlreadyTime"]:
                sql_FindAllTime = "select userAllTime for Users where userAccount='" + \
                    str(Js["userAccount"])+"'"
                print(sql_FindAllTime)
                cursor.execute(sql_FindAllTime)
                AllTime = cursor.fetchone()[0]
            sql_AddAllTime = "update User set userAllTime=" + \
                str(AllTime+Js["jobSetDuration"])
            print(sql_AddAllTime)
            sql_AddRecord = "insert into "+Js["userAccount"]+"Job values('"+str(Js["jobName"])+"'"+","+"'"\
                + str(Js["jobSetDuration"])+"'"+","+"'"+str(Js["jobCreateTime"])+"'"+","+"'"+str(Js["jobStartTime"])+"'"+","+"'"\
                + str(Js["jobEndTime"])+"'"+","+"'"+str(Js["jobSusbendTime"])+"'"+","+"'"+str(Js["jobType"])+"'"+","+"'"\
                + str(Js["jobScene"])+"'"+","+"'"+str(Js["jobAlreadyTime"])+"'"+","+"'"+str(Js["Concentration"])+"'"+","+"'"\
                + str(Js["Circularity"])+"')"
            cursor.execute(sql_FindAllTime)
            print(sql_AddRecord)
            cursor.execute(sql_AddRecord)
            self.conn.commit()
            return "True"
        except Exception as e:
            print("JobFinish error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)
    
    def ToDoUpdata(self,Js):
        try:
            sql_CoverUserToDo = "delete from "+Js["userAccount"]+"Todo"
            print(sql_CoverUserToDo)
            cursor = self.conn.cursor()
            cursor.execute(sql_CoverUserToDo)
            self.conn.commit()
            for ToDo in Js["data"]:
                sql_ToDoUpdata = "insert into "+ToDo["userAccount"]+"Todo values('"+str(ToDo["jobName"])+"'"+","+"'"\
                    + str(ToDo["jobSetDuration"])+"'"+","+"'"+str(ToDo["jobCreateTime"])+"'"+","+"'"+str(ToDo["jobStartTime"])+"'"+","+"'"\
                    + str(ToDo["jobEndTime"])+"'"+","+"'"+str(ToDo["jobSusbendTime"])+"'"+","+str(ToDo["jobType"])+","+"'"\
                    + str(ToDo["jobScene"])+"'"+","+"'"+str(ToDo["jobAlreadyTime"])+"'"+","+"'"+str(ToDo["currentTime"])+"'"+","\
                    + str(ToDo["currentlngtion"])+str(ToDo["currentSlience"]
                                                        )+","+str(ToDo["tolgnition"])+")"
                print(sql_ToDoUpdata)
                cursor.execute(sql_ToDoUpdata)
            self.conn.commit()
            return "True"
        except Exception as e:
            print("ToDoUpdata error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)

    def TolngitionLeardBoard(self,Js):
        try:
            sql_Select = "select userAccount from Users where currentlngtion>" + \
            str(Js["currentlngtion"])
            print(sql_Select)
            cursor = self.conn.cursor()
            cursor.execute(sql_Select)
            num = 0
            for Users in cursor:
                num = num+1
            self.conn.commit()
            return str(num)
        except Exception as e:
            print("TolngitionLeaderBoard error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)

    def AllTimeLeaderBoard(self,Js):
        try:
            sql_SelectTime = "select userAllTime from Users where userAccount='" + \
            str(Js["userAccount"])+"'"
            cursor = self.conn.cursor()
            cursor.execute(sql_SelectTime)
            Time = cursor.fetchone()[0]
            sql_Select = "select userAccount from Users where userAllTime>" + \
                str(Time)
            print(sql_Select)
            cursor = self.conn.cursor()
            cursor.execute(sql_Select)
            num = 0
            for Users in cursor:
                num = num+1
            self.conn.commit()
            return str(num)
        except Exception as e:
            print("AllTimeLeaderBoard error ", str(e))
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)


if __name__ == '__main__':
    mysqlutil = MysqlUtil()
