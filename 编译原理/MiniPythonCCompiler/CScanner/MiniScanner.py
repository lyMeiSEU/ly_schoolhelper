import re
from tkinter import * 
from tkinter.filedialog import askdirectory,askopenfilename

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
            token.run(line,output, True)
            #写入指定文件中
            #token.write_file(token.run(line, False), "D:/results.txt")
            token.lineno += 1
        output.close()

if __name__=='__main__':
    root = Tk()
    root.title('词法分析')
    root.resizable(0, 0)
    path = StringVar() 
    token = Token()
    Label(root,text = "目标路径:").grid(row = 0, column = 0) 
    Entry(root, textvariable = path).grid(row = 0, column = 1) 
    Button(root, text = "路径选择", command = token.changefile).grid(row = 0, column = 2)
    Button(root,text='词法分析',command= token.analysis).grid(row = 0,column = 3)
    root.mainloop()