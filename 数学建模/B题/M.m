%�����񣬲��ӱ���ж���x,y
B1 = xlsread('2019B����.xlsx','����1');
B2 = xlsread('2019B����.xlsx','����2');

x=B2(:, 1:10);
y=B2(:, 12);

CoreNum=8;
parpool(CoreNum);

%��������x,y�Ƿ�������̬�ֲ�
if jbtest(x(:,1))==0
    disp("����������ݷ�����̬�ֲ�")
end
if jbtest(y(:,1))==0
    disp("��Ʒ�����������ݷ�����̬�ֲ�")
end

%����Լ�⣬���ÿ�������������
for i=1:9
    corr(x(:,i),x(:,i+1))
end

%��һ��
[xn,PSx1] = mapminmax(x');
[yn,PSy] = mapminmax(y');

% ����������
net = feedforwardnet(5,'trainbr');
net = train(net, xn, yn); % ������Ҫִ�ж�Σ�����������Ϊ1000������Ҫִ����һ��

average_x=mean(x,1)
stanard_x=std(x,1)
data_pool_1=normrnd(average_x(1),stanard_x(1),100) %����ÿ���������100��������̬�ֲ������ݲ��������ݳ�
data_pool_2=normrnd(average_x(2),stanard_x(2),100)
data_pool_3=normrnd(average_x(3),stanard_x(3),100)
data_pool_4=normrnd(average_x(4),stanard_x(4),100)
data_pool_5=normrnd(average_x(5),stanard_x(5),100)
data_pool_6=normrnd(average_x(6),stanard_x(6),100)
data_pool_7=normrnd(average_x(7),stanard_x(7),100)
data_pool_8=normrnd(average_x(8),stanard_x(8),100)
data_pool_9=normrnd(average_x(9),stanard_x(9),100)
data_pool_10=normrnd(average_x(10),stanard_x(10),100)
T=1;
a=0.99; %ģ���˻��㷨����ϵ��
max=0;
r1=-3;  %���ó�ʼֵ���ڼ�¼����x1~x10ѡ��
r2=-3;
r3=-3;
r4=-3;
r5=-3;
r6=-3;
r7=-3;
r8=-3;
r9=-3;
r10=-3;

x1 = B1(1,:);%���ڼ�¼���1~10���ۼ�
x2 = B1(2,:);
x3 = B1(3,:);
x4 = B1(4,:);
x5 = B1(5,:);
x6 = B1(6,:);
x7 = B1(7,:);
x8 = B1(8,:);
x9 = B1(9,:);
x10 = B1(10,:);

current_profit=0; %�ϴε�����Ԥ������
flag=0; %�Ƿ�ֹͣ�˻�
spmd
    for x_1=-2:2
        for x_2=-2:2
            for x_3=-2:2
                for x_4=-2:2
                    for x_5=-2:2
                        for x_6=-2:2
                            for x_7=-2:2
                                for x_8=-2:2
                                    for x_9=-2:2
                                        for x_10=-2:2
                                            if(T<0.5)
                                                flag=1;
                                            end
                                            cost=x1(abs(x_1)+1)+x2(abs(x_2)+1)+x3(abs(x_3)+1)+x4(abs(x_4)+1)+x5(abs(x_5)+1)+x6(abs(x_6)+1)+x7(abs(x_7)+1)+x8(abs(x_8)+1)+x9(abs(x_9)+1)+x10(abs(x_10)+1);
                                            X(1)=average_x(1)*(1+0.05*x_1); %����x1~x10�ĳ�ʼֵ���������ݳ�
                                            X(2)=average_x(2)*(1+0.05*x_2);
                                            X(3)=average_x(3)*(1+0.05*x_3);
                                            X(4)=average_x(4)*(1+0.05*x_4);
                                            X(5)=average_x(5)*(1+0.05*x_5);
                                            X(6)=average_x(6)*(1+0.05*x_6);
                                            X(7)=average_x(7)*(1+0.05*x_7);
                                            X(8)=average_x(8)*(1+0.05*x_8);
                                            X(9)=average_x(9)*(1+0.05*x_9);
                                            X(10)=average_x(10)*(1+0.05*x_10);
                                            XN=mapminmax('apply',X',PSx1);
                                            fitnumber=0; %��¼���ؿ����㷨����������
                                            y_nowall=mapminmax('reverse',sim(net,XN),PSy); %���������
                                            for i=1:100
                                                if (data_pool_1(i)<average_x(1)*(1+0.05*(x_1+1)))&&(average_x(1)*(1+0.05*x_1)<data_pool_1(i))
                                                    fitnumber=fitnumber+1;
                                                    X(1)=data_pool_1(i);
                                                    y_nowall=y_nowall+mapminmax('reverse',sim(net,XN),PSy);
                                                end
                                            end
                                            for i=1:100
                                                if (data_pool_2(i)<average_x(2)*(1+0.05*(x_2+1)))&&(average_x(2)*(1+0.05*x_2)<data_pool_1(i))
                                                    fitnumber=fitnumber+1;
                                                    X(2)=data_pool_1(i);
                                                    y_nowall=y_nowall+mapminmax('reverse',sim(net,XN),PSy);
                                                end
                                            end
                                            for i=1:100
                                                if (data_pool_3(i)<average_x(3)*(1+0.05*(x_3+1)))&&(average_x(3)*(1+0.05*x_3)<data_pool_1(i))
                                                    fitnumber=fitnumber+1;
                                                    X(3)=data_pool_1(i);
                                                    y_nowall=y_nowall+mapminmax('reverse',sim(net,XN),PSy);
                                                end
                                            end
                                            for i=1:100
                                                if (data_pool_4(i)<average_x(4)*(1+0.05*(x_4+1)))&&(average_x(4)*(1+0.05*x_4)<data_pool_1(i))
                                                    fitnumber=fitnumber+1;
                                                    X(4)=data_pool_1(i);
                                                    y_nowall=y_nowall+mapminmax('reverse',sim(net,XN),PSy);
                                                end
                                            end
                                            for i=1:100
                                                if (data_pool_5(i)<average_x(5)*(1+0.05*(x_5+1)))&&(average_x(5)*(1+0.05*x_5)<data_pool_1(i))
                                                    fitnumber=fitnumber+1;
                                                    X(5)=data_pool_1(i);
                                                    y_nowall=y_nowall+mapminmax('reverse',sim(net,XN),PSy);
                                                end
                                            end
                                            for i=1:100
                                                if (data_pool_6(i)<average_x(6)*(1+0.05*(x_6+1)))&&(average_x(6)*(1+0.05*x_6)<data_pool_1(i))
                                                    fitnumber=fitnumber+1;
                                                    X(6)=data_pool_1(i);
                                                    y_nowall=y_nowall+mapminmax('reverse',sim(net,XN),PSy);
                                                end
                                            end
                                            for i=1:100
                                                if (data_pool_7(i)<average_x(7)*(1+0.05*(x_7+1)))&&(average_x(7)*(1+0.05*x_7)<data_pool_1(i))
                                                    fitnumber=fitnumber+1;
                                                    X(7)=data_pool_1(i);
                                                    y_nowall=y_nowall+mapminmax('reverse',sim(net,XN),PSy);
                                                end
                                            end
                                            for i=1:100
                                                if (data_pool_8(i)<average_x(8)*(1+0.05*(x_8+1)))&&(average_x(8)*(1+0.05*x_8)<data_pool_1(i))
                                                    fitnumber=fitnumber+1;
                                                    X(8)=data_pool_1(i);
                                                    y_nowall=y_nowall+mapminmax('reverse',sim(net,XN),PSy);
                                                end
                                            end
                                            for i=1:100
                                                if (data_pool_9(i)<average_x(9)*(1+0.05*(x_9+1)))&&(average_x(9)*(1+0.05*x_9)<data_pool_1(i))
                                                    fitnumber=fitnumber+1;
                                                    X(9)=data_pool_1(i);
                                                    y_nowall=y_nowall+mapminmax('reverse',sim(net,XN),PSy);
                                                end
                                            end
                                            for i=1:100
                                                if (data_pool_10(i)<average_x(10)*(1+0.05*(x_10+1)))&&(average_x(10)*(1+0.05*x_10)<data_pool_1(i))
                                                    fitnumber=fitnumber+1;
                                                    X(10)=data_pool_1(i);
                                                    y_nowall=y_nowall+mapminmax('reverse',sim(net,XN),PSy);
                                                end
                                            end
                                            y_now=y_nowall/fitnumber; %��������
                                            if (182<y_now)&&(y_now<186) %��ƷΪA��
                                                profit=1500-cost;
                                            elseif ((178<y_now)&&(y_now<182))||((186<y_now)&&(y_now<190)) %��ƷΪB��
                                                profit=1200-cost;
                                            elseif ((165<y_now)&&(y_now<178))||((190<y_now)&&(y_now<197)) %��ƷΪC��
                                                profit=800-cost;
                                            else
                                                profit=600-cost; %��ƷΪD��
                                            end
                                            if profit<current_profit
                                                T=a*T;
                                            end
                                            if profit>=max
                                                T=1;
                                                max=profit;
                                                disp("max="+max)
                                                r1=x_1;
                                                r2=x_2;
                                                r3=x_3;
                                                r4=x_4;
                                                r5=x_5;
                                                r6=x_6;
                                                r7=x_7;
                                                r8=x_8;
                                                r9=x_9;
                                                r10=x_10; %��¼��ǰ�������ʱ���ȡֵ
                                                %disp(XN)
                                                disp("gradex1~x10 ")
                                                disp([x_1,x_2,x_3,x_4,x_5,x_6,x_7,x_8,x_9,x_10]')
                                                if T<0.5
                                                    flag=1; %�ж��Ƿ������˻�����
                                                end
                                            end
                                            if profit>current_profit
                                                current_profit=profit; %��¼��һ������
                                            end
                                        end
                                        if flag==1
                                            break;
                                        end
                                    end
                                    if flag==1
                                        break;
                                    end
                                end
                                if flag==1
                                    break;
                                end
                            end
                            if flag==1
                                break;
                            end
                        end
                        if flag==1
                            break;
                        end
                    end
                    if flag==1
                        break;
                    end
                end
                if flag==1
                    break;
                end
            end
            if flag==1
                break;
            end
        end
        if flag==1
            break;
        end
    end
end
fprintf("10 end"+"\n")
delete(gcp)
