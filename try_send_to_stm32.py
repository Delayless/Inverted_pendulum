#!/usr/bin/env python3
import PID
import serial
import struct


def recv(ser_temp):
	"""
	读数据
	"""
	ser_temp.read_until(terminator=b'^')
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
	pwm = 1300.42
	pwm = Limit_Amplitude(pwm)
	if pwm < 0:
		signal = '-'
	else:
		signal = '+'

	ser_buff.write('^'.encode())  # 帧首
	ser_buff.write(signal.encode())
	ser_buff.write(str(int(abs(pwm))).encode())
	ser_buff.write('$'.encode())  # 帧尾


def main():
	ser.flushOutput()
	ser.flushInput()
	Angle_Median = 3085  # 平衡时的角度读数 
	Position_Zero=10000
	Position_count = 0
	Balance_circle =  PID.IncrementalPID(50, 0, 244)
	Position_circle = PID.PositionPID(-25, 0, -600)
		

if __name__ == "__main__":
	ser = serial.Serial('/dev/ttyUSB0', 128000, timeout=0.05)

	main()
	while True:
		Data_Process(ser)


