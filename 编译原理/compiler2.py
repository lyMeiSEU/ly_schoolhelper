from collections import defaultdict

input=["S->a","S->^","S->(T)","T->SU","U->,SU","U->ε"]
unTerminate=[]
Terminate=[]
for item in input:
    if item[0:item.find('->')] not in unTerminate:
        unTerminate.append(item[0:item.find('->')])
for item in input:
    where=item.find('->')+2
    for i in range(where,len(item)):
        if item[i] not in Terminate and item[i]not in unTerminate:
            Terminate.append(item[i])

def calculateFirst(S):
    first=[]
    for item in input:
        if item[0]==S:
            if item[3] in Terminate and item[3] not in first:
                first.append(item[3])
            elif item[3] in unTerminate:
                next=calculateFirst(item[3])
                for nextitem in next:
                    first.append(nextitem)
    return first
def calculateFollow(state,S):
    follow=[]
    for item in state:
        if S in item[1]:
            where=item[1].find(S)
            if where+1==len(item[1]):
                for itemindex in item[3]:
                    follow.append(itemindex)
            elif item[1][where+1] in Terminate:
                follow.append(item[1][where+1])
            else:
                next=calculateFirst(item[1][where+1])
                for nextindex in next:
                    follow.append(nextindex)
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
                state[i][3].append(calculateFollow(state,state[i][0]))
                break
            #如果是非终结符且未到末尾
            elif state[i][1][state[i][2]] in unTerminate:
                for item in input:
                    #遍历产生式寻找开始符相同产生式
                    if item[0]==state[i][1][state[i][2]]:
                        flag=-1
                        #判断产生式是否存在
                        for j in range(0,len(state)):
                            if item[3:len(item)] in state[j]:
                                flag=j
                        #计算预测符
                        #print(state[i][1][state[i][2]])
                        #未到末尾
                        if state[i][2]+1!=len(state[i][1]):
                            if state[i][1][state[i][2]+1] in Terminate:
                                predit=[state[i][1][state[i][2]+1]]
                            else:
                                wherenow=1
                                predit=[]
                                #一直出现ε
                                while state[i][2]+wherenow!=len(state[i][1]):
                                    if 'ε' in input[i in range(0,len(input))] and state[i][2]==input[i in range(0,len(input))][0]:
                                        predit.append(calculateFirst(state[i][1][state[i][2]+wherenow])-'ε'+calculateFirst(state[i][1][state[i][2]+wherenow+1]))
                                        wherenow=wherenow+1
                                    else:
                                        break
                                if state[i][2]+wherenow==len(state[i][1]):
                                    predit.append(calculateFollow(state[i][0]))
                        #到达末尾
                        else:
                            predit=calculateFollow(state,state[i][0])
                        #不存在则加入产生式
                        if flag==-1:
                            state.append([item[0],item[3:len(item)],0,predit])
                        #存在则补充预测符
                        elif state[j][3]!=predit:
                            for predititem in predit:
                                if predititem not in state[j][3]:
                                    state[j][3].append(predititem)
    removeEmpty(state)

#删除空预测符
def removeEmpty(state):
    for item in state:
        if item[3]==[]:
            item[3].append('$R')
        if [] in item[3]:
            item[3].remove([])

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
        print(together[sameExtension])
        for value in together[sameExtension]:
            if value[2]+1<=len(value[1]):
                value[2]=value[2]+1
            elif calculateFollow(stateNew,value[0]) not in value[3]:
                value[3].append(calculateFollow(stateNew,value[0]))
            stateNew.append(value)
            innerExtension(stateNew)
        innerExtension(stateNew)
        removeEmpty(stateNew)
        states.append([stateNew,sameExtension])
    return states

Statements=[]
changeTable=defaultdict(list)
#state[[左,右,点位置,预测符]]
state0=[['S\'',unTerminate[0],0,['$R']]]
state=[]
innerExtension(state0)
Statements.append(state0)
i=0
while(i<len(Statements)):
    state=betweenExtension(Statements[i])
    if state==[]:
        i=i+1
        continue
    flag=False
    for item in state:
        if item[0] not in Statements:
            Statements.append(item[0])
            if i not in changeTable.keys():
                changeTable[i]=[[len(Statements)-1,item[1]]]
            else:
                changeTable[i].append([len(Statements)-1,item[1]])
    i=i+1
print(Statements)