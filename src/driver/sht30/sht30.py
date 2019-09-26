import smbus

import time

i2c = smbus.SMBus(1)

addr=0x44

i2c.write_byte_data(addr, 0x23, 0x34)

time.sleep(0.5)

while 1:

    i2c.write_byte_data(addr, 0xe0, 0x0)

    data = i2c.read_i2c_block_data(addr, 0x0, 6)

    rawT = ((data[0]) << 8) | (data[1])
    rawR = ((data[3]) << 8) | (data[4])

    temp = -45 + rawT * 175.72 / 65535
    print (str(temp) +"C")

    RH = 100 * rawR / 65535
    print (str(RH) +"%")

    time.sleep(1)

    print ("*************")
