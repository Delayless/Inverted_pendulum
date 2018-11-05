#!/usr/bin/env python3

class IncrementalPID:
	"""
	增量式PID,入口参数为比例系数,积分系数,微分系数
	"""
	def __init__(self, P, I, D):
		self.Kp = P
		self.Ki = I
		self.Kd = D
		
		self.PIDOutput = 0.0
		self.Error = 0.0
		self.LastError = 0.0
		self.PrevError = 0.0

	
	def Calculate(self, feedback, Setpoint):
		"""
		入口参数为反馈值,给定值
		记住有需要可能要同步时钟
		还可以加优化算法,如
		"""	
		self.Error = feedback - Setpoint
		self.PIDOutput += self.Kp*(self.Error-self.LastError) + self.Ki*self.Error + self.Kd*(self.Error-2*self.LastError+self.PrevError) 
		self.PrevError = self.LastError
		self.LastError = self.Error
	

class PositionPID:
	"""
	位置式PID优化算法有积分分离,微分先行(如果经常突变下面圆盘位置的话)
	还可以用串级PID
	商家下位机原来用的是一阶低通滤波算法(平滑滤波)
	"""
	def __init__(self, P, I, D):
		self.Kp = P
		self.Ki = I
		self.Kd = D

		self.PIDOutput = 0.0
		self.Error = 0.0
		self.LastError = 0.0
		self.Integral = 0.0	

	def Calculate(self, feedback, SetPoint):
		"""
		位置式PID
		debug by peng
		刚开始的LastError和PrevError不应该用0
		但是也不应该用此刻的Error,否则该开始比例作用和微分作用都失效
		而唯一作用的积分作用前期又很弱
		"""
		self.Error = feedback - SetPoint
		self.Integral += self.Error
		self.PIDOutput = self.Kp*self.Error + self.Ki*self.Integral + self.Kp*(self.Error-self.LastError)
		self.LastError = self.Error


def Limit_Amplitude(pwm):
	Amplitude = 6900
	if pwm < -Amplitude:
		pwm = -Amplitude
	if pwm > Amplitude:
		pwm = Amplitude

	return pwm
