from collections import defaultdict

#input=["S->a","S->^","S->(T)","T->SU","U->,SU","U->ε"]
#input=['S->Aa','S->bAc','S->Bc','S->bBa','A->d','B->d']
fileRead=open('inputCFG.txt','r',encoding='utf-8')
lines=fileRead.readlines()
input=[]
for line in lines:
    input.append(line.strip('\n'))
StartSymble=''
unTerminate=defaultdict(list)
Terminate=[]
for item in input:
    if StartSymble=='':
        StartSymble=item[0]
    if item[0:item.find('->')] not in unTerminate.keys():
        unTerminate[item[0:item.find('->')]]=[item]
    else:
        unTerminate[item[0:item.find('->')]].append(item)
for item in input:
    where=item.find('->')+2
    for i in range(where,len(item)):
        if item[i] not in Terminate and item[i]not in unTerminate.keys():
            Terminate.append(item[i])

def calculateFirst(S):
    first=[]
    for item in input:
        if item[0]==S:
            if item[3] in Terminate and item[3] not in first:
                first.append(item[3])
            elif item[3] in unTerminate.keys():
                next=calculateFirst(item[3])
                for nextitem in next:
                    if nextitem not in first:
                        first.append(nextitem)
    if first==[]:
        first.append(S)
    set(first)
    return first
def calculateFollow(state,S):
    follow=[]
    for item in state:
        if S in item[1]:
            where=item[1].find(S)
            if where+1==len(item[1]):
                for itemindex in item[3]:
                    if itemindex not in follow:
                        follow.append(itemindex)
            elif item[1][where+1] in Terminate and item[1][where+1] not in follow:
                follow.append(item[1][where+1])
            else:
                next=calculateFirst(item[1][where+1])
                for nextindex in next:
                    if nextindex not in follow:
                        follow.append(nextindex)
    set(follow)
    return follow

                  #状态 到达符号 状态内第几个产生式的扩展

#状态内扩展
def innerExtension(state):
    lastLen=0
    while(len(state)!=lastLen):
        lastLen=len(state)
        #遍历寻找未扩展完非终结符
        for i in range(0,len(state)):
            #到达末尾
            if state[i][2]+1>len(state[i][1]):
                break
            #如果是非终结符且未到末尾
            elif state[i][1][state[i][2]] in unTerminate.keys():
                follow=calculateFollow(state,state[i][1][state[i][2]])
                for item in unTerminate[state[i][1][state[i][2]]]:
                    if [item[0],item[3:len(item)],0,follow] not in state:
                        state.append([item[0],item[3:len(item)],0,follow])
        removeEmpty(state)
        removeRedundancy(state)

#删除空预测符
def removeEmpty(state):
    for item in state:
        if [] in item[3]:
            item[3].remove([])

def removeRedundancy(state):
    for item in state:
        set(item[3])


#状态间扩展
def betweenExtension(state):
    states=[]
    together=defaultdict(list)
    for item in state:
        if item[2]==len(item[1]):
            return []
        if item[1][item[2]] not in together.keys():
            together[item[1][item[2]]]=[item]
        else:
            together[item[1][item[2]]].append(item)
    for sameExtension in together.keys():
        stateNew=[]
        #print(together[sameExtension])
        for value in together[sameExtension]:
            if value[2]+1<=len(value[1]):
                stateNew.append([value[0],value[1],value[2]+1,value[3]])
                innerExtension(stateNew)
            else:
                stateNew.append([value[0],value[1],value[2]+1,set(calculateFollow(stateNew,value[0]))])
        innerExtension(stateNew)
        removeEmpty(stateNew)
        removeRedundancy(stateNew)
        states.append([stateNew,sameExtension])
    return states

Statements=[]
changeTable=defaultdict(list)
statusTable=defaultdict(list)
#state[[左,右,点位置,预测符]]
state0=[['S\'',StartSymble,0,['$R']]]
state=[]
innerExtension(state0)
Statements.append(state0)
i=0
while(i<len(Statements)):
    stateExt=Statements[i]
    state=betweenExtension(Statements[i])
    Statements[i]=stateExt
    if state==[]:
        i=i+1
        continue
    flag=False
    for item in state:
        for object in item[0]:
            if len(object[3])>=3:
                True
        if item[0] not in Statements:
            Statements.append(item[0])
            if i not in changeTable.keys():
                changeTable[i]=[[len(Statements)-1,item[1]]]
            else:
                changeTable[i].append([len(Statements)-1,item[1]])
    i=i+1
for i in range(0,len(Statements)):
    for j in range(0,len(Statements[i])):
        if Statements[i][j][2]==len(Statements[i][j][1]):
            if i not in statusTable.keys():
                for item in range(0,len(input)):
                    #print(input[item][input[item].find('->')+2:len(input[item])])
                    if Statements[i][j][1]==StartSymble:
                        statusTable[i]=[[Statements[i][j][3],0]]
                    if input[item][input[item].find('->')+2:len(input[item])]==Statements[i][j][1]:
                        statusTable[i]=[[Statements[i][j][3],item+1]]
                        break
#print(statusTable)
#print(changeTable)
fileState=open('stateTable.txt','w+')
print('---------------状态列表-----------------')
fileState.write('---------------状态列表-----------------'+'\n')
for i in range(0,len(Statements)):
    state=[]
    for item in Statements[i]:
        state.append(item)
    print(state)
    fileState.write(str(state)+'\n')
AllTable=[]
fileChange=open('PPT.txt','w+')
print('---------------状态转换表-----------------')
fileChange.write('---------------状态转换表-----------------'+'\n')
#构造表格每个状态一行
for i in range(0,len(Statements)):
    AllTable.append([])
    if i in changeTable.keys():
        for item in changeTable[i]:
            if item[1] in  Terminate:
                AllTable[i].append(['s',item[0],item[1]])
            elif item[1] in unTerminate.keys():
                AllTable[i].append(['S',item[0],item[1]])
    if i in statusTable.keys():
        for item in statusTable[i]:
            AllTable[i].append(['r',item[1],item[0][0]])
            if len(item[0])>1:
                for j in range(1,len(item[0])):
                    AllTable[i].append(['r',item[1],item[0][j]])
for i in range(0,len(Statements)):
    print(str('state%s:'%(str(i))+str(AllTable[i])))
    fileChange.write(str('state%s:'%(str(i))+str(AllTable[i]))+'\n')
fileChange.close()
fileState.close()
fileRead=open('inputStream.txt','r',encoding='utf-8')
inputStream=fileRead.readline().strip('\n')
stateStack=[]
intiger=0
inputStream=inputStream+'$'
stateStack.append(['$',0])
fileRedundancy=open('redundancySequence.txt','w+')
while(True):
    item=inputStream[intiger]
    state=stateStack[len(stateStack)-1][1]
    flag=True
    for action in AllTable[state]:
        if action[2][0]==item:
            flag=False
            break
    if flag:
        print('Stream Error, Can Not Analyse!')
        fileRedundancy.write('Stream Error, Can Not Analyse!\n')
        break
    if action[1]==0 and action[2]=='$R':
        print('Success!') 
        fileRedundancy.write('Success!\n')
        break
    if action[0]=='r':
        print(input[action[1]-1])
        fileRedundancy.write(str(input[action[1]-1])+'\n')
        for i in range(3,len(input[action[1]-1])):
            stateStack.pop()
        for nextState in AllTable[stateStack[len(stateStack)-1][1]]:
            if nextState[2]==input[action[1]-1][0]:
                break
        nextState=[input[action[1]-1][0],nextState[1]]
        stateStack.append(nextState)
    if action[0]=='s':
        nextState=[action[2],action[1]]
        stateStack.append(nextState)
        intiger=intiger+1
fileRedundancy.close()