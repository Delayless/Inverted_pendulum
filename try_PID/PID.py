#!/usr/bin/env python3
import time

class IncrementalPID:
	"""增量式PID,入口参数为比例系数、积分系数、微分系数"""
	def __init__(self, P, I, D):
		self.Kp = P
		self.Ki = I
		self.Kd = D

		self.PIDOutput = 0.0  # PID输出,可直接控制执行元件,或者滤波等处理后再控制
		self.Error = 0.0
		self.LastError = 0.0  # e[-1]
		self.PrevError = 0.0  # 上上次的偏差e[-2]


	def Calculator(self, feedback, SetPoint):
		"""计算"""		
		# debug by peng
		# 如果实际应用还要考虑采样时间,毕竟上位机python的时钟和下位机stm32的时钟不一定同步
		self.Error = SetPoint - feedback  # 控制仪表上是这么做的,但是自控原理上为相反数 
		self.PIDOutput += self.Kp*(self.Error-self.LastError) + self.Ki*self.Error + self.Kd*(self.Error-2*self.LastError+self.PrevError)
		self.PrevError = self.LastError
		self.LastError = self.Error
		
	
def main():
	"""
	print these parameter:
						peak_time 峰值时间
						adjust_time # alias setting_time
						没法求调节时间,除非创建一个队列,先进先出,队列中的数要全部满足98%的条件
						暂时不想花这个时间,就只求峰值时间和峰值(超调量自然就出来了)吧
						
	"""
	SetPoint = 1  # 目标值

	"""
	if SetPoint > Step_Response.PIDOutput:
		up_flag = 1  # 响应曲线刚开始应该为上升曲线 
		down_flag = 0	
	else:
		up_flag = 0
		down_flag = 1  # 响应曲线刚开始应该为下降曲线 

	first_blood = 1  
	Last_Output = Step_Response.PIDOutput
	start_time = time.time()
	"""
	while True:
		# 单位反馈,直接将PID输出作为feedback
		Step_Response.Calculator(Step_Response.PIDOutput, SetPoint)
		if Step_Response.PIDOutput > 10000000000:  # 震荡了,阶跃响应中Kp为1时临界振荡
			break

		"""
		# 求峰值时间,输出第一次下降就是峰值(上升曲线中),vice versa
		if Last_Output > Step_Response.PIDOutput and up_flag == 1 and first_blood == 1:
			peak_time = time.time() - start_time
			peak_value = Last_Output
			up_flag = 0
			first_blood = 0
		if  Last_Output < Step_Response.PIDOutput and down_flag == 1 and first_blood == 1:
			peak_time = time.time() - start_time
			peak_value = Last_Output
			down_flag = 0
			first_blood = 0
		Last_Output = Step_Response.PIDOutput
		"""
		print(Step_Response.PIDOutput)


if __name__ == "__main__":
	Step_Response = IncrementalPID(0.999, 0, 0.00)
	main()
