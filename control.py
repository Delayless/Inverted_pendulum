#!/usr/bin/env python3
import PID
import serial
import struct


def recv(ser_temp):
	"""
	读数据
	"""
	ser_temp.read_until(terminator='$')
	data_temp = ser_temp.read(8)

	return data_temp


def Limit_Amplitude(pwm):
        Amplitude = 6900
        if pwm < -Amplitude:
                pwm = -Amplitude
        if pwm > Amplitude:
                pwm = Amplitude
        return pwm


def Data_Process(ser_buff):
	Sensor_data = recv(ser_buff)  # 接收从下位机发送过来的传感器的数据	
		
	list_s = []
	# 将拆成四个字节存储的浮点数转换成相应的十六进制数并连成字符串
	# 这组十六进制数转成byte类型的话就可以通过struct.unpack转换成浮点数了
	for i in Sensor_data[0:4]:
		list_s.append("%02x"%i)  # 一个字节需要两个十六进制数表示
	Angle_str = ''.join(list_s)  # 可以把list列表转成字符串
	
	list_s = []
	for i in Sensor_data[4:8]:
		list_s.append("%02x"%i) 
	Position_str = ''.join(list_s) 
	
	# 将四个字节的byte类型数据转换成浮点数
	Angle = struct.unpack('f', Angle_str)[0]
	Position = struct.unpack('f', Position_str)[0]
	Balance_PWM = Balancee_circle.Calculate(Angle, Angle_Median)	
	if Position_count > 4:
		Position_PWM = Position_circle.Calculate(Position, Position_Zero)
		Position_count = 0	 
	
	pwm = Balance_PWM - Position_PWM
	pwm = Limit_Amplitude(pwm)
	if pwm < 0:
		signal = '+'
	else:
		signal = '-'
	ser_buff.write(signal)
	ser_buff.write(pwm)
	

def main():
	ser = serial.Serial('/dev/ttyUSB0', 128000, timeout=0.2)
	
	# 先只控制直立环,后面再解决两个环的通讯协议问题
	ser.flushOutput()
	ser.flushInput()
	Angle_Median = 3085  # 平衡时的角度读数 
	Position_Zero=10000
	Position_count = 0
	Balance_circle =  PID.Incremental(50, 0, 244)
	Position_circle = PID.Position(-25, 0, -600)
		

if __name__ == "__main__":
	main()
	while True:
		Data_Process(ser)


