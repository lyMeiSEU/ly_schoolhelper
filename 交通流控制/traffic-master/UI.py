#!/usr/bin/python
# -*- coding: UTF-8 -*-

import tkinter as tk
import simulation as s
import Q_table

# 按钮列表
Buttonlist = []


# 运行环境输入参数
class Parameter():
    def __init__(self):
        self.learning_rate = tk.StringVar()
        self.reward_decay = tk.StringVar()
        self.e_greedy_begin = tk.StringVar()
        self.e_greedy_end = tk.StringVar()
        self.explore_time = tk.StringVar()
        self.num_of_sim = tk.StringVar()


# 数据库输入参数
class DBdata():
    def __init__(self):
        self.test = 1


def open_sumo(num):
    s.simulation(num)


class TopFrame(object):
    def __init__(self, root):
        self.root = root
        self.frame = tk.Frame(root, bg='black')

        self.title = tk.Label(self.frame, height=2, text='人工智能路网能力提升策略及工具系统', font=('微软雅黑', 16), bg='black', fg='white')
        self.title.pack(side='left', padx=20)

        # self.button = tk.Button(self.frame, text='打开仿真软件', command=open_sumo, bg='black', fg='white', bd=0)
        # self.button.pack(side='right', padx=20)

        self.frame.pack(side='top', fill='x')


class Window(object):
    """docstring for Window"""

    def __init__(self, window):
        self.window = window

        TopFrame(window)

        self.left_frame = tk.Frame(window)
        self.right_frame = tk.Frame(window, bg="white")

        self.left_frame.pack(side='left')
        self.right_frame.pack(side='right', expand=True)
        self.index_page()

        # 运行环境输入参数
        self.parameter = Parameter()

        button_text = ['首页',
                       '运行环境输入',
                       '仿真模型参数输入',
                       '仿真学习状态监控',
                       '运行结果输出',
                       '信控控制策略输出',
                       ]
        for i in range(len(button_text)):
            Button(self, i, button_text[i])

    def change_right_frame(self, i):
        self.right_frame.destroy()
        self.right_frame = tk.Frame(window, bg="white", width=400)
        self.right_frame.pack(side='right', expand=True)

        if i == 0:
            self.index_page()
            return
        elif i == 1:
            self.parameter_page()
            return
        elif i == 2:
            self.DB_page()
            return
        elif i == 3:
            self.start_page()
            return

        text = '你点击了第' + str(i) + '个按钮'
        label = tk.Label(self.right_frame, text=text, bg='green', font=('微软雅黑', 20))
        label.pack()

    # 首页贴图
    def index_page(self):
        im = tk.PhotoImage(file="welcome.png")
        label = tk.Label(self.right_frame, image=im, borderwidth=0)
        label.image = im
        label.pack()

    # 运行环境输入页面
    def parameter_page(self):
        Input(self.right_frame, 'Learning Rate:', self.parameter.learning_rate, 0)
        self.parameter.learning_rate.set("0.01")
        Input(self.right_frame, 'Reward Decay:', self.parameter.reward_decay, 1)
        self.parameter.reward_decay.set("0.9")
        Input(self.right_frame, 'E Greedy Begin:', self.parameter.e_greedy_begin, 2)
        self.parameter.e_greedy_begin.set("0.1")
        Input(self.right_frame, 'E Greedy End:', self.parameter.e_greedy_end, 3)
        self.parameter.e_greedy_end.set("0.9")
        Input(self.right_frame, 'Explore Time:', self.parameter.explore_time, 4)
        self.parameter.explore_time.set("5000000")
        Input(self.right_frame, 'Number of Simulation:', self.parameter.num_of_sim, 5)
        self.parameter.num_of_sim.set("20000")
        button = tk.Button(self.right_frame, text="确定", font=('微软雅黑', 12), bg='#CCCCCC', bd=0, width=12, height=2,
                           command=self.change_parameter)
        button.grid(row=6, column=0, sticky=tk.E, padx=20, pady=10)

    # 修改运输参数
    def change_parameter(self):
        Q_table.m_learning_rate = float(self.parameter.learning_rate.get())
        Q_table.m_explore_time = float(self.parameter.explore_time.get())
        Q_table.m_e_greddy_end = float(self.parameter.e_greedy_end.get())
        Q_table.m_e_greedy_begin = float(self.parameter.e_greedy_begin.get())
        Q_table.m_reward_decay = float(self.parameter.reward_decay.get())
        tk.messagebox.showinfo(title='提示', message='修改成功')

    # 数据库输入页面
    def DB_page(self):
        print("test")

    # 数据库输入页面
    def start_page(self):
        label1 = tk.Label(self.right_frame, text="参数预览", font=('微软雅黑', 14, 'bold'), bg="white")
        label1.grid(row=0, column=0, sticky=tk.N, padx=20, pady=10)
        label1 = tk.Label(self.right_frame, text="Learning Rate:    " + str(self.parameter.learning_rate.get()),
                          font=('微软雅黑', 14), bg="white")
        label1.grid(row=1, column=0, sticky=tk.N, padx=20, pady=10)
        label2 = tk.Label(self.right_frame, text="Reward Decay:    " + str(self.parameter.reward_decay.get()),
                          font=('微软雅黑', 14), bg="white")
        label2.grid(row=2, column=0, sticky=tk.N, padx=20, pady=10)
        label3 = tk.Label(self.right_frame, text="E Greedy Begin:    " + str(self.parameter.e_greedy_begin.get()),
                          font=('微软雅黑', 14), bg="white")
        label3.grid(row=3, column=0, sticky=tk.N, padx=20, pady=10)
        label4 = tk.Label(self.right_frame, text="E Greedy End:    " + str(self.parameter.e_greedy_end.get()),
                          font=('微软雅黑', 14), bg="white")
        label4.grid(row=4, column=0, sticky=tk.N, padx=20, pady=10)
        label5 = tk.Label(self.right_frame, text="Explore Time:    " + str(self.parameter.explore_time.get()),
                          font=('微软雅黑', 14), bg="white")
        label5.grid(row=5, column=0, sticky=tk.N, padx=20, pady=10)
        label6 = tk.Label(self.right_frame, text="Number of Simulation:    " + str(self.parameter.num_of_sim.get()),
                          font=('微软雅黑', 14), bg="white")
        label6.grid(row=6, column=0, sticky=tk.N, padx=20, pady=10)
        button = tk.Button(self.right_frame, text='打开仿真软件', font=('微软雅黑', 12),
                           command=lambda: open_sumo(int(self.parameter.num_of_sim.get())),
                           bg='#CCCCCC', bd=0, width=12, height=2)
        button.grid(row=7, column=0, sticky=tk.N, padx=20, pady=10)


class Button(object):
    """docstring for Button"""

    def __init__(self, root, index, btn_text):
        self.root = root
        self.index = index
        b = tk.Button(self.root.left_frame, text=btn_text, font=('微软雅黑', 12), bg='#CCCCCC', bd=0, width=20, height=3,
                      command=lambda: self.show_frame(b))
        b.pack(anchor='w', fill='x')
        Buttonlist.append(b)

    def show_frame(self, button):
        button.config(bg="white")
        for m in Buttonlist:
            if m != button:
                m.config(bg="#CCCCCC")
        i = self.index
        print('i:', i)
        self.root.change_right_frame(i)


class Input(object):
    def __init__(self, root, text, parameter, r):
        self.text = text
        # self.frame = tk.Frame(root, bg="white")
        self.label = tk.Label(root, text=text, font=('微软雅黑', 14), bg="white")
        self.label.grid(row=r, column=0, sticky=tk.E, padx=20, pady=10)
        self.entry = tk.Entry(root, textvariable=parameter, font=('Arial', 14), bg="white")
        self.entry.grid(row=r, column=1, sticky=tk.W, padx=20, pady=10)


# self.frame.pack(anchor='w', padx=30, pady=10)
# self.frame.pack(side="top")


if __name__ == '__main__':
    window = tk.Tk()
    window.title('人工智能路网能力提升策略及工具系统')
    window.config(bg="white")
    window.minsize(800, 400)
    window.resizable(width="false", height="false")
    Window(window)

    window.mainloop()
# s.simulation()
