import pickle,os,time,argparse
#本实验仅针对c语言程序的简单计算，如需处理其他场景，请更换RE和保留字表、操作符表、分隔符表以及对。故没有考虑字符和字符串等类型。

#自己定义的REs,只支持//注释
#@表示数字
RE1="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_||||||||||||||||||||||||||||||||||||||||||||||||||||abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_||||||||||||||||||||||||||||||||||||||||||||||||||||@|*·"
#变量名命名规则字母或下划线开头，由字母下划线数字组成的串,本想将字母统一用一个字母表示，但由于保留字含有字母，无法用一个字母表示，否则无法生成DFA。
RE2="@@*·"#整数
RE3="@@*·.·@·@*·"#浮点数
RE4="+-&/=||||"#操作符
RE5="()|"#分隔符
RE6="int··float····double·····short····long···||||"#保留字


# 判断一个字符是否是一个数字
def isDigit(charactor):
    return charactor.isdigit()

# 判断一个字符是否是一个字母
def isLetter(charactor):
    return charactor.isalpha()

# 是否空白符
def isSpace(charactor):
    return charactor in " \n\r\t"
class Token(object):
    #初始化
    def __init__(self):
         #存储分词的列表
        self.results = []
        #行号
        self.lineno = 1
        #关键字
        self.keywords = ['auto', 'struct', 'if', 'else', 'for', 'do', 'while', 'const',
            'int', 'double', 'float', 'long', 'char', 'short', 'unsigned',
            'switch', 'break', 'defalut', 'continue', 'return', 'void', 'static',
            'auto', 'enum', 'register', 'typeof', 'volatile', 'union', 'extern']
        '''
        regex中：*表示从0-， +表示1-， ？表示0-1。对应的需要转义
        { 表示限定符表达式开始的地方 \{
        () 标记一个子表达式的开始和结束位置。子表达式可以获取共以后使用：\( \)
        r表示原生字符串。
        '''  
        Keyword = r'(?P<Keyword>(auto){1}|(double){1}|(int){1}|(if){1}|' \
		          r'(#include){1}|(return){1}|(char){1}|(stdio\.h){1}|(const){1})'
         #运算符
        Operator = r'(?P<Operator>\+\+|\+=|\+|--|-=|-|\*=|/=|/|%=|%)'
         #分隔符/界符
        Separator = r'(?P<Separator>[,:\{}:)(<>])'
         #数字: 例如：1 1.9
        Number = r'(?P<Number>\d+[.]?\d+)'
         #变量名 不能使用关键字命名
        ID = r'(?P<ID>[a-zA-Z_][a-zA-Z_0-9]*)'
         #方法名 {1} 重复n次
        Method = r'(?P<Method>(main){1}|(printf){1})'
         #错误  \S 匹配任意不是空白符的字符
         #Error = r'(?P<Error>.*\S+)'
        Error = r'\"(?P<Error>.*)\"'
         #注释  ^匹配行的开始 .匹配换行符以外的任意字符 \r回车符 \n换行符
        Annotation = r'(?P<Annotation>/\*(.|[\r\n])*/|//[^\n]*)'
        
         #进行组装，将上述正则表达式以逻辑的方式进行拼接, 按照一定的逻辑顺序
         # compile函数用于编译正则表达式，生成一个正则表达式对象
        self.ID=[]
        self.patterns = re.compile('|'.join([Annotation, Keyword, Method, ID, Number, Separator, Operator, Error]))
    #读文件
    def read_file(self,filename):
        with open(filename, "r") as f_input:
            return [line.strip() for line in f_input]
    #结果写入文件
    def write_file(self, lines, filename = 'D:/results.txt'):
        with open(filename, "a") as f_output:
                for line in lines:
                    if line:
                        f_output.write(line)
                    else:
                        continue
    def get_token(self, line):
        #finditer : 在字符串中找到正则表达式所匹配的所有字串， 并把他们作为一个迭代器返回
        for match in re.finditer(self.patterns, line):
            #group()：匹配的整个表达式的字符 # yield 关键字：类似return ，返回的是一个生成器，generator
            innercode=-5
            if match.group() in self.keywords:
                innercode=1+self.keywords.index(match.group())
            elif match.lastgroup=='Operater':
                innercode=0
            elif match.lastgroup=='Separator':
                innercode=-1
            elif match.lastgroup=='Method':
                innercode=-2
            elif match.lastgroup=='Error':
                innercode=-3
            elif match.lastgroup=='Annotation':
                innercode=-4
            elif match.lastgroup=='Number':
                innercode=match.group()
            elif match.lastgroup=='ID':
                self.ID.append(match.group())
                innercode=len(self.ID)
            yield (match.group(),match.lastgroup,innercode)
    def run(self, line, output, flag=True):
        output.write("line %3d :"%self.lineno+'\n')
        for token in self.get_token(line):
            if flag:
                print ("line %3d :" % self.lineno, token)
                output.write(str([token])+'\n')
    def changefile(self,filepath="Test.c"):
        filepath=askopenfilename(title='打开文件', filetypes=[('All Files', '*')])
        global path
        path.set(filepath)
        self.lines = self.read_file(filepath)
    def analysis(self):
        self.__init__()
        output=open("a.l","w")
        for line in self.lines:
            self.run(line,output, True)
            #写入指定文件中
            #token.write_file(token.run(line, False), "D:/results.txt")
            self.lineno += 1
        output.close()


# 标识符表 0
identitierTable = list()
# 整数常量表 5000
integerTable = list()
# 浮点数常量表 10000
floatTable = list()

# 各种表的起始偏移
# 整数常量表
integerId = 5000
# 浮点数常量表
floatId = 10000
# 错误代码
errorId = 404

#栈
class Stack(object):
    def __init__(self):
        self.stack = []

    def push(self, value):    # 进栈
        self.stack.append(value)

    def pop(self):  #出栈
        if self.stack:
            self.stack.pop()
        else:
            raise LookupError('stack is empty!')

    def is_empty(self): # 如果栈为空
        return bool(self.stack)

    def top(self): 
        #取出目前stack中最新的元素
        return self.stack[-1]
    def top2(self):
        #取出倒数第二个元素
        return self.stack[-2]


#生成NFA 汤普森算法 用字典表表示自动机

def NFA(s,label):#s:RE，label:相应的类别
    base_states=list()#NFA的节点，用字典表表示每个节点，key表示边，null表示ε边
    start=list()#起始节点
    end=list()#终节点
    stack = Stack()
    num_state = 0#NFA总节点数
    elements = set()#所有的边

    for i in s:
        if i == '|':
            start.append(num_state+1)#连接后，删除原来的两个起始节点，生成的新起始节点放入列表
            start.remove(stack.top()[0])
            start.remove(stack.top2()[0])
            a={"null":[stack.top()[0],stack.top2()[0]]}#将新起始节点连接两条ε边到原来两个起始节点
            b={"null":[]}

            end.append(num_state+2)#删除终节点，生成新的终节点
            end.remove(stack.top()[1])
            end.remove(stack.top2()[1])
            base_states[stack.top()[1]-1]["null"].append(num_state+2)#原来的终节点连接ε边到新的终节点
            base_states[stack.top2()[1]-1]["null"].append(num_state+2)
            stack.pop()#弹出原来的关系
            stack.pop()

            base_states.append(a.copy())#新产生的两个节点
            base_states.append(b.copy())

            stack.push((num_state+1,num_state+2))#压进新的关系
            num_state = num_state+2
        elif i == '·':
            base_states[stack.top2()[1]-1]["null"].append(stack.top()[0])#连接两个·关系
            start.remove(stack.top()[0])
            end.remove(stack.top2()[1])
            a = (stack.top2()[0],stack.top()[1])
            stack.pop()
            stack.pop()
            stack.push(a)
        elif i == '*':
            start.remove(stack.top()[0])
            end.remove(stack.top()[1])
            base_states[stack.top()[1]-1]["null"].append(stack.top()[0])
            base_states[stack.top()[1]-1]["null"].append(num_state+2)

            a = {"null":[num_state+2,stack.top()[0]]}
            b = {"null":[]}

            start.append(num_state+1)
            end.append(num_state+2)

            base_states.append(a.copy())
            base_states.append(b.copy())

            stack.pop()
            stack.push((num_state+1,num_state+2))
            num_state = num_state+2

        else:
            a={i:num_state+2,"null":[]}#生成两个新节点 a—i—>b
            b={"null":[]}
            start.append(num_state+1)#把两个节点分别送入开始结束节点
            end.append(num_state+2)

            base_states.append(a.copy())#把两个节点的拷贝放入节点集
            base_states.append(b.copy())
            stack.push((num_state+1,num_state+2))#将该元素开始结束节点关系压栈
            num_state = num_state+2
            elements.add(i)

    return base_states,start,end,num_state,elements,label

#合并n个NFA
def nNFA(num_states,NFAs,starts,ends,labels):
#(list:每个NFA的节点个数，list:每个NFA字典，starts:每个NFA的起始节点,ends:每个NFA的终节点,labels:每个NFA的标签)
    for i in range(1,len(NFAs)):#将NFA节点数量合并， 如[3,3,3]变为[3,6,9]
        for j in range(0,len(NFAs[i])):
            for k in NFAs[i][j]:
                if k != "null":
                    for m in range(0,i):
                        NFAs[i][j][k] = NFAs[i][j][k] + num_states[m]
                else:
                    if NFAs[i][j][k]:
                        for m in range(0,i):
                            for n in range(0,len(NFAs[i][j][k])):
                                NFAs[i][j][k][n] = NFAs[i][j][k][n] + num_states[m]
        for x in range(i,len(starts)):#将开始节点数量合并
            for y in range(0,len(starts[x])):
                    starts[x][y] = starts[x][y] + num_states[i-1]
        for x in range(i,len(ends)):#将终节点数量合并
            for y in range(0,len(ends[x])):
                    ends[x][y] = ends[x][y] + num_states[i-1]
    all_NFAs=[]#将列表的n个元素整合到一起
    for i in NFAs:
        all_NFAs.extend(i)
    all_starts=[]
    for i in starts:
        all_starts.extend(i)
    all_ends=[]
    for i in ends:
        all_ends.extend(i)

    label=dict()#将终节点和相应的label对应起来
    for i in range(0,len(labels)):
        label.update({labels[i]:all_ends[i]})

    return all_NFAs,set(all_starts),set(all_ends),label


def expand(core,NFA):
#(核，NFA自动机)
#扩展节点ε状态
    num = len(core)
    for i in list(core):
            for j in NFA[i-1]["null"]:
                core.add(j)
    while len(core) != num:
        num = len(core)
        for i in list(core):
            for j in NFA[i-1]["null"]:
                core.add(j)
    return core

def expand_edge(edge,core,NFA):
#(要走的边，核，NFA自动机)
#当前状态通过edge后的下一状态以及其扩展
    num=0;
    newcore = set()
    for i in list(core):
        p = NFA[i-1].get(edge,-1)
        if p != -1:
            newcore.add(p)
    newcore = expand(newcore,NFA)
    return newcore

def DFA(NFA,start,end,elements,labels):
#(NFA自动机,起始节点list,终节点list,所有边的list,状态对应dict)
#将n个NFA的整合转化为DFA
    DFA_states_num=list()#DFA节点数字标记
    DFA_states_dict=list()#DFA节点边关系
    DFA_states_dict.append(dict())#起始节点
    num_DFA_states = 1#DFA节点数
    current_row=1;#当前表格行数
    queue = list()#队列处理各个状态

    queue.append(expand(start,NFA))#队列的第一个元素为起始状态
    DFA_states_num.append(queue[0].copy())#起始状态加入DFA状态

    while queue:
        for i in elements:#遍历所有边
            x = expand_edge(i,queue[0],NFA)
            if x:
                if x not in DFA_states_num:#对新的节点

                    DFA_states_num.append(x.copy())
                    queue.append(x.copy())

                    num_DFA_states = num_DFA_states+1;
                    DFA_states_dict[current_row-1][i]=num_DFA_states
                    DFA_states_dict.append(dict())#创建一个新的待填行
                else:#已存在的节点
                    DFA_states_dict[current_row-1][i]=DFA_states_num.index(x)+1
        current_row = current_row+1
        del(queue[0])

    other_states=set()#放置其他节点，由于终节点有意义不需要化简，只需对其他节点化简，但发现我使用的例子这里只有两个节点，不需化简，故没有做。
    end_states=set()
    start_states=set()
    for i in range(0,len(DFA_states_num)):
        for j in end:
            if j not in DFA_states_num[i]:
                other_states.add(i)
                continue
            end_states.add(i)
            other_states.discard(i)
            break

    for i in range(0,len(DFA_states_num)):
        for j in start:
            if j in DFA_states_num[i]:
                start_states.add(i)

    all_labels=dict()
    for i in labels:
        all_labels[i]=[]
        for j in end_states:
            if labels[i] in DFA_states_num[j]:
                all_labels[i].append(j)
    for i in all_labels["basicWord"]:#同属于标识符和保留字的终态，标识符的终态应被删去，如{3,7}，3表示标识符，7表示保留字，应变为{7}
        all_labels["identitier"].remove(i)

    return start_states,end_states,DFA_states_dict,all_labels

#对于用于举例的RE1~RE6构造DFA
def make():
    a1,b1,c1,d1,e1,f1 = NFA(RE1,"identitier")
    a2,b2,c2,d2,e2,f2 = NFA(RE2,"integer")
    a3,b3,c3,d3,e3,f3 = NFA(RE3,"float")
    a4,b4,c4,d4,e4,f4 = NFA(RE4,"operator")
    a5,b5,c5,d5,e5,f5 = NFA(RE5,"separator")
    a6,b6,c6,d6,e6,f6 = NFA(RE6,"basicWord")

    a=[a1,a2,a3,a4,a5,a6]
    b=[b1,b2,b3,b4,b5,b6]
    c=[c1,c2,c3,c4,c5,c6]
    d=[d1,d2,d3,d4,d5,d6]
    f=[f1,f2,f3,f4,f5,f6]

    e=[]
    e.extend(e1)
    e.extend(e2)
    e.extend(e3)
    e.extend(e4)
    e.extend(e5)
    e.extend(e6)

    all_NFAs,all_starts,all_ends,labels=nNFA(d,a,b,c,f)

    start_states,end_states,DFA_states_dict,all_labels=DFA(all_NFAs,all_starts,all_ends,list(set(e)),labels)

    rw1 = start_states
    rw2 = end_states
    rw3 = DFA_states_dict
    rw4 = all_labels
    output_hal1 = open("start_states.pkl", 'wb')
    output_hal2 = open("end_states.pkl", 'wb')
    output_hal3 = open("DFA_states_dict.pkl", 'wb')
    output_hal4 = open("all_labels.pkl", 'wb')
    str1 = pickle.dumps(rw1)
    str2 = pickle.dumps(rw2)
    str3 = pickle.dumps(rw3)
    str4 = pickle.dumps(rw4)
    output_hal1.write(str1)
    output_hal2.write(str2)
    output_hal3.write(str3)
    output_hal4.write(str4)
    output_hal1.close()
    output_hal2.close()
    output_hal3.close()
    output_hal4.close()

#扫描器
def ThompsonScanner():
#(内容，指向当前字符的指针，行数，生成的文件名)
    global text,pointer,line,filename
    print([text,pointer,line,filename])
    pointer =0
    start_states = set()
    end_states = set()
    DFA_states_dict = list()
    all_labels = dict()
    with open("start_states.pkl",'rb') as f1:
        start_states = pickle.loads(f1.read())
    for i in start_states:
        start_state = i
    with open("end_states.pkl",'rb') as f2:
        end_states = pickle.loads(f2.read())
    with open("DFA_states_dict.pkl",'rb') as f3:
        DFA_states_dict = pickle.loads(f3.read())
    with open("all_labels.pkl",'rb') as f4:
        all_labels = pickle.loads(f4.read())
    with open(filename+"_.txt",'w') as file:
        file.write("=========line {}=========\n".format(line))#第一行
        print("=========line {}=========\n".format(line))
        while pointer >= 0 and pointer < len(text) :
            buffer=""
            if isSpace(text[pointer]) or text[pointer]==';' or text[pointer]=='\\':#遇到换行空格或注释
                if text[pointer]=='\n':#换行转到下一行
                    line += 1;
                    file.write("=========line {}=========\n".format(line))
                    print("=========line {}=========\n".format(line))
                if text[pointer]==';':
                    if pointer+1 < len(text):
                        if text[pointer+1]=='\n':#；后换行
                            pointer += 1
                            line += 1;
                            file.write("=========line {}=========\n".format(line))
                            print("=========line {}=========\n".format(line))
                        elif text[pointer+1]!='\\':#；后接字母报错
                            file.write("can not have charactors after ; ！")
                            print("can not have charactors after ; ！")
                            pointer = -2
                        elif pointer+2 < len(text):
                            if text[pointer+1]=='\\' and text[pointer+2]=='\\':#注释被忽略
                                while text[pointer]!='\n' and pointer<len(text):
                                    pointer += 1
                                line += 1
                                file.write("=========line {}=========\n".format(line))
                                print("=========line {}=========\n".format(line))
                            else:#单\后出现其他字母报错
                                file.write("can not have charactors after ; ！")
                                print("can not have charactors after ; ！")
                                pointer = -2
                if pointer!=-1 and text[pointer]=='\\' and text[pointer+1]=='\\':#此处可能会越界
                    while text[pointer]!='\n' and pointer<len(text):
                        pointer += 1
                    line += 1
                    file.write("=========line {}=========\n".format(line))
                    print("=========line {}=========\n".format(line))
                pointer += 1
            else:#是有效字符
                buffer += text[pointer]

                if isDigit(text[pointer]):#数字走@边
                    p1 = DFA_states_dict[start_state].get('@',0)-1
                else:
                    p1 = DFA_states_dict[start_state].get(text[pointer],0)-1
                p=p1#p1为当前状态，p为尝试走下一状态，不一定有

                pointer += 1
                while pointer < len(text) and p!=-1:
                    if isSpace(text[pointer]) or text[pointer]==';':
                        break
                    else:
                        if isDigit(text[pointer]):
                            p = DFA_states_dict[p1].get('@',0)-1
                        else:
                            p = DFA_states_dict[p1].get(text[pointer],0)-1
                        if p != -1:
                            p1=p
                            buffer += text[pointer]
                            pointer += 1
                        else:
                            break

                if p==-1:
                    if p1 not in end_states:
                        classNumber = errorId
                        buffer = buffer + " has Wrong charactor. eorrorID: "+str(classNumber)#含有非法字符
                        pointer = -2
                    else:
                        p = p1
                elif p not in end_states:
                    classNumber = errorId
                    buffer = buffer + " is not a legal name. eorrorID: "+str(classNumber)#当前变量名不正确
                    pointer = -2
                if p in all_labels["identitier"]:#判断终态属于哪一个label
                    if buffer in identitierTable:
                        buffer = token(buffer,"identitier",identitierTable.index(buffer))
                    else:
                        identitierTable.append(buffer)
                        buffer = token(buffer,"identitier",identitierTable.index(buffer))
                elif p in all_labels["integer"]:
                    if buffer in integerTable:
                        buffer = token(buffer,"integer",integerId+integerTable.index(buffer))
                    else:
                        integerTable.append(buffer)
                        buffer = token(buffer,"integer",integerId+integerTable.index(buffer))
                elif p in all_labels["float"]:
                    if buffer in floatTable:
                        buffer = token(buffer,"float",floatId+floatTable.index(buffer))
                    else:
                        floatTable.append(buffer)
                        buffer = token(buffer,"float",floatId+floatTable.index(buffer))
                elif p in all_labels["operator"]:
                    buffer = token(buffer,"operator","---")
                elif p in all_labels["separator"]:
                    buffer = token(buffer,"separator","---")
                elif p in all_labels["basicWord"]:
                    buffer = token(buffer,"basicWord","---")
                file.write(buffer+'\n')
                print(buffer+'\n')


# token格式
def token(name_,class_,id_):
    return "("+name_+","+class_+","+str(id_)+")"

# 参数解析
def argParse():
    parser = argparse.ArgumentParser()
    # Add argument
    parser.add_argument('--file','-f', type=str, help='path of the file to analyze or \'make\' to make')
    return parser.parse_args()

# 读取内容
def readFile(fileName):
    with open(fileName,encoding = 'utf-8')as f:
        text = f.read()
    return text

import re
from tkinter import * 
from tkinter.filedialog import askdirectory,askopenfilename

def changefile():
    filepath=askopenfilename(title='打开文件', filetypes=[('All Files', '*')])
    global path
    path.set(filepath)
    global text,Token
    text=readFile(filepath)
    Token.lines=Token.read_file(filepath)

if __name__ == "__main__":
    root = Tk()
    fileName = os.path.basename("cpp.txt")
    filename = fileName[:fileName.index('.')]
    pointer = 0
    line = 1
    text = readFile("cpp.c")
    root.title('词法分析')
    root.resizable(0, 0)
    path = StringVar()  
    Token = Token()
    Label(root,text = "目标路径:").grid(row = 0, column = 0) 
    Entry(root, textvariable = path).grid(row = 0, column = 1) 
    Button(root, text = "路径选择", command = changefile).grid(row = 0, column = 2)
    Button(root,text='词法分析(汤普森)',command= ThompsonScanner).grid(row = 0,column = 3)
    Button(root,text='词法分析(RE工作包)',command= Token.analysis).grid(row = 0,column = 4)
    root.mainloop()