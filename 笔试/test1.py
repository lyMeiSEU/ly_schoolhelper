import xlrd
from collections import defaultdict

file_Man1="people1"
file_Man2="people2"
file_Man3="people3"

table1=xlrd.open_workbook(file_Man1+'.xlsx')
table2=xlrd.open_workbook(file_Man2+'.xlsx')
table3=xlrd.open_workbook(file_Man3+'.xlsx')

# 获得前两年的人的ID
def getYear(table):
    sheet1 = table.sheets()[0]
    rows1 = sheet1.nrows
    year1=[]
    for row in range(0, rows1):
        rowvalue = sheet1.row_values(row)
        year1.append(rowvalue[2])
    return year1

# 找到参加三年的所有人，按照"年龄":[人1的所有信息, 人2的所有信息...]存储
def FindMan(table,year1,year2):
    sheet1 = table.sheets()[0]
    rows1 = sheet1.nrows
    AllMan=defaultdict(list)
    year=list(set(year1).intersection(set(year2)))
    for row in range(0, rows1):
        rowvalue = sheet1.row_values(row)
        age=rowvalue[1]
        if rowvalue[2] in year:
            AllMan[str(age)].append([rowvalue[0],rowvalue[1],rowvalue[2],rowvalue[3],rowvalue[4]])
    return AllMan

year1=getYear(table1)
year2=getYear(table2)
AllMan=FindMan(table3,year1,year2)
# 按照年龄key倒序排序
keyList=sorted(AllMan.keys(),reverse=True)

# 倒序打印
for key in keyList:
    for item in AllMan[key]:
        print(item)