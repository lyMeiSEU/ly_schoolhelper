import pymysql
import json

class MysqlUtil(object):
    name = 'MysqlUtil'
    """
    use for connect Mysql ,need sql words
    """

    def __init__(self):
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
        sql_TableCreate="create table if not exists Users(userNumber char(10), userType int, userAccount char(20), userPassword char(20), userAllTime float, userAim text)"
        sql_UserCheck="select * from Users"
        cursor=self.conn.cursor()
        cursor.execute(sql_TableCreate)
        cursor.execute(sql_UserCheck)
        worldList=[]
        for Users in cursor:
            worldList.append([Users])

        print(worldList)
        self.conn.commit()
    
    def mysqlAll(self,js):
        try:
            global mysqlutil
            Js=json.loads(js)
            if Js["method"]=="userLogin":
                sql_PasswordSearch="select userPassword from Users where userAccount='"+Js["data"][0]["userAccount"]+"'"
                print(sql)
                cursor=self.conn.cursor()
                cursor.execute(sql_PasswordSearch)
                if Js["data"][0]["userPassword"]==cursor.fetchone()[0]:
                    return "True"
                else:
                    return "False"
            elif Js["method"]=="userCreate":
                sqlQ="select * from Users where userAccount='"+Js["data"][0]["userAccount"]+"'"
                print(sqlQ)
                cursor=self.conn.cursor()
                cursor.execute(sqlQ)
                worldList=[]
                for Users in cursor:
                    worldList.append([Users])
                if worldList!=[]:
                    return "User exist"
                sql="insert into Users(userType,userAccount,userPassword) values('"+str(Js["data"][0]["userType"])+\
                    "'"+","+"'"+Js["data"][0]["userAccount"]+"'"+","+"'"+Js["data"][0]["userPassword"]+"');"
                print(sql)
                cursor=self.conn.cursor()
                cursor.execute(sql)
                self.conn.commit()
                sql_UserJobTable="create table if not exists "+Js["data"][0]["userAccount"]+"Job(jobName String, "+\
                    "jobSetDuration datetime, jobCreateTime datetime, jobStartTime datetime, jobEndTime datetime, jobSusbendTime datetime, "\
                    "jobType int, jobScene String, jobAlreadyTime datetime, Concentration int, Circularity int);"
                print(sql_UserJobTable)
                cursor=self.conn.cursor()
                cursor.execute(sql_UserJobTable)
                self.conn.commit()
                sql_UserToDoTable="create table if not exists "+Js["data"][0]["userAccount"]+"Todo(jobName String, "+\
                    "jobSetDuration datetime, jobCreateTime datetime, jobStartTime datetime, jobEndTime datetime, jobSusbendTime datetime, "\
                    "jobType int, jobScene String, jobAlreadyTime datetime, currentTime datetime, currentlngtion double, currentSlience double, tolgnition double);"
                print(sql_UserToDoTable)
                cursor=self.conn.cursor()
                cursor.execute(sql_UserToDoTable)
                self.conn.commit()
                return "True"
            elif Js["method"]=="userJobFinish":
                sql_AddRecord="insert into "+Js["userAccount"]+"Job values('"+str(Js["jobName"])+"'"+","+"'"\
                    +str(Js["jobSetDuration"])+"'"+","+"'"+str(Js["jobCreateTime"])+"'"+","+"'"+str(Js["jobStartTime"])+"'"+","+"'"\
                    +str(Js["jobEndTime"])+"'"+","+"'"+str(Js["jobSusbendTime"])+"'"+","+"'"+str(Js["jobType"])+"'"+","+"'"\
                    +str(Js["jobScene"])+"'"+","+"'"+str(Js["jobAlreadyTime"])+"'"+","+"'"+str(Js["Concentration"])+"'"+","+"'"\
                    +str(Js["Circularity"])+"')"
                print(sql_AddRecord)
                cursor=self.conn.cursor()
                cursor.execute(sql_AddRecord)
                self.conn.commit()             
                return "True"
            elif Js["method"]=="userToDoUpdate":
                sql_CoverUserToDo="delete from "+Js["userAccount"]+"Todo"
                print(sql_CoverUserToDo)
                cursor=self.conn.cursor()
                cursor.execute(sql_CoverUserToDo)
                self.conn.commit()
                for ToDo in Js["data"]:
                    sql_ToDoUpdata="insert into "+ToDo["userAccount"]+"Todo values('"+str(ToDo["jobName"])+"'"+","+"'"\
                    +str(ToDo["jobSetDuration"])+"'"+","+"'"+str(ToDo["jobCreateTime"])+"'"+","+"'"+str(ToDo["jobStartTime"])+"'"+","+"'"\
                    +str(ToDo["jobEndTime"])+"'"+","+"'"+str(ToDo["jobSusbendTime"])+"'"+","+"'"+str(ToDo["jobType"])+"'"+","+"'"\
                    +str(ToDo["jobScene"])+"'"+","+"'"+str(ToDo["jobAlreadyTime"])+"'"+","+"'"+str(ToDo["currentTime"])+"'"+","+"'"\
                    +str(ToDo["currentlngtion"])+str(ToDo["currentSlience"])+"'"+","+"'"+str(ToDo["Tolgnition"])+"')"
                    print(sql_ToDoUpdata)
                    cursor.execute(sql_ToDoUpdata)
                self.conn.commit()
                return "True"

        except Exception as e:
            print("mysql error ", e)
            mysqlutil = MysqlUtil()
            self.conn.rollback()
            return str(e)
        finally:
            # 关闭游标和连接
            self.cur.close()
            self.conn.close()
            mysqlutil = MysqlUtil()

if __name__ == '__main__':
    mysqlutil = MysqlUtil()