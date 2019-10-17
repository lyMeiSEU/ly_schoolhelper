import pypyodbc
import random as random
from Name import Name
path = u'Driver={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=' + "./university.accdb"
db = pypyodbc.win_connect_mdb(path)

sql_prof="select pname from prof where dname in (select dname from dept where numphds<'50')"
sql_worstStudent="select sname from student where gpa=(select min(gpa) from student)"
sql_CSclass='SELECT section.cno, section.sectno, Avg(gpa) AS gpa之平均值 \
    FROM section, enroll, student WHERE ( (section.dname ="Computer Sciences") and (section.cno = enroll.cno) and (enroll.sid=student.sid)  )  \
    GROUP BY section.cno, section.sectno'
sql_lessThenSix='SELECT (select cname from course where cno=section.cno and dname=section.dname) AS cname, section.cno, section.sectno\
    FROM [section], enroll\
    WHERE (section.cno = enroll.cno )\
    GROUP BY section.cno, section.sectno, section.dname\
    HAVING count(*) < 6'
sql_mostClassStudent='SELECT sid, sname\
    FROM (SELECT sid, (select sname from student where sid=enroll.sid) AS sname , count(*) As num\
    FROM enroll\
    group by enroll.sid)  AS b\
    WHERE b.num = ( select max(num) From (SELECT sid, (select sname from student where sid=enroll.sid) AS sname , count(*) As num\
    FROM enroll group by enroll.sid) )'

cur=db.cursor()
sql_MaxID="select max(ID) from student"
cur.execute(sql_MaxID)
num=int(cur.fetchall()[0][0])
with open('student.txt','w') as f:
    for i in range(num+1,5001):
        sname=Name.gen_two_words(split=' ', lowercase=False)
        if random.randint(0,1)==1:
            sex='m'
        else:
            sex='f'
        age=random.randint(15,80)
        year=random.randint(1,6)
        gpa=round(random.uniform(1,5),1)
        sql_Insert="insert into student values(%s,%s,'%s','%s',%s,%s,%s)"%(str(i),str(i),str(sname),str(sex),str(age),str(year),str(gpa))
        #print(sql_Insert)
        cur.execute(sql_Insert)
        #print('%s\t"%s"\t"%s"\t%s\t%s\t%s\n'%(str(i),str(sname),str(sex),str(age),str(year),str(gpa)))
        f.write('%s\t"%s"\t"%s"\t%s\t%s\t%s\n'%(str(i),str(sname),str(sex),str(age),str(year),str(gpa)))