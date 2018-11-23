#!/usr/bin/env python3
import PID
import serial
import struct


def recv(ser_temp):
	"""
	读数据
	"""
	ser_temp.read_until(terminator='$', size=50)
	data_temp = ser_temp.read(9)

	return data_temp


def Limit_Amplitude(pwm):
        Amplitude = 6900
        if pwm < -Amplitude:
                pwm = -Amplitude
        if pwm > Amplitude:
                pwm = Amplitude
        return pwm


def Data_Process(ser_buff):
	# 从下位机接过来的数据为bytes类型
	Sensor_data = recv(ser_buff)  # 接收从下位机发送过来的传感器的数据	
	# 将组合在一起的两组拆成四个字节存储的浮点数切片回两组数据
	# 将byte类型的数据切片后就可以通过struct.unpack转换成浮点数了
	Angle_byt = Sensor_data[0:4] 
	Position_byt = Sensor_data[4:8] 
	
	# 将四个字节的byte类型数据转换成浮点数
	Angle = struct.unpack('f', Angle_byt)[0]
	Position = struct.unpack('f', Position_byt)[0]
	print(Angle)
	print(Position)
	"""
	Balance_PWM = Balancee_circle.Calculate(Angle, Angle_Median)	
	if Position_count > 4:
		Position_PWM = Position_circle.Calculate(Position, Position_Zero)
		Position_count = 0	 
	
	pwm = Balance_PWM - Position_PWM
	pwm = Limit_Amplitude(pwm)
	if pwm < 0:
		signal = '-'
	else:
		signal = '+'
	ser_buff.write(signal)
	ser_buff.write(pwm)
	"""

def main():
	ser.flushOutput()
	ser.flushInput()
	Angle_Median = 3085  # 平衡时的角度读数 
	Position_Zero=10000
	Position_count = 0
	Balance_circle =  PID.IncrementalPID(50, 0, 244)
	Position_circle = PID.PositionPID(-25, 0, -600)
		

if __name__ == "__main__":
	ser = serial.Serial('/dev/ttyUSB0', 128000, timeout=0.01) # 0.2s还没接到数据基本上程序就GG了
	main()
	while True:
		Data_Process(ser)


