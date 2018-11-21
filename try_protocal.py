# 全部复杂化了,留下了是程序中有各种数制转换的参考价值
# 其实可以直接通过对bytes类型数据切片,然后就可以直接用struct.unpack转换成浮点数了

    # 从下位机接过来的数据为bytes类型
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
    # 换一种方法如下,一句话等同于上面四行
    # Position_str = Sensor_data[4:8].hex()


    # 将四个字节的byte类型数据转换成浮点数
    Angle = struct.unpack('f', Angle_str)[0]
    Position = struct.unpack('f', Position_str)[0]
    print(type(Angle_str))
    print(Angle_str)

