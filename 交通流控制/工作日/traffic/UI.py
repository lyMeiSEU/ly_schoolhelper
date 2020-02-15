#!/usr/bin/python
# -*- coding: UTF-8 -*-
 
import tkinter as tk

class Window(object):
	"""docstring for Window"""
	def __init__(self,window):
		self.window=window

		self.title=tk.Label(window,text='人工智能路网能力提升策略及工具系统',font=('微软雅黑',28))
		self.left_frame=tk.Frame(window)
		self.right_frame=tk.Frame(window)

		self.title.pack(side='top')
		self.left_frame.pack(side='left')
		self.right_frame.pack(side='right',expand=True)
		self.index_page()

		button_text=['首页',
			'运行环境输入',
			'仿真模型参数输入',
			'强化学习参数输入',
			'运行状态监控',
			'仿真学习状态监控',
			'学习收敛情况监控',
			'运行结果输出',
			'信控控制策略输出',
			'信控结果评价输出',
			'运行结果管理',
			'知识库管理']
		for i in range(len(button_text)):
			Button(self,i,button_text[i])

	def change_right_frame(self,i):
		self.right_frame.destroy()
		self.right_frame=tk.Frame(window)
		self.right_frame.pack(side='right',expand=True)

		if i==0:
			self.index_page()
			return

		text='你点击了第'+str(i)+'个按钮'
		label=tk.Label(self.right_frame,text=text,bg='green',font=('微软雅黑',20))
		label.pack()

	def index_page(self):
		tk.Label(self.right_frame, text='参数1:', font=('微软雅黑', 14)).pack()
		tk.Entry(self.right_frame,font=('Arial', 14)).pack()
		tk.Label(self.right_frame, text='参数2:', font=('微软雅黑', 14)).pack()
		tk.Entry(self.right_frame,font=('Arial', 14)).pack()


class Button(object):
	"""docstring for Button"""
	def __init__(self,root,index,btn_text):
		self.root=root
		self.index=index
		b=tk.Button(self.root.left_frame ,text=btn_text,width=30,height=2,command=self.show_frame)
		b.pack(anchor='w')

	def show_frame(self):
		i=self.index
		print('i:',i)
		self.root.change_right_frame(i)




if __name__ == '__main__':
	window = tk.Tk()
	window.title('人工智能路网能力提升策略及工具系统')
	window.minsize(800,500)
	Window(window)

	window.mainloop()