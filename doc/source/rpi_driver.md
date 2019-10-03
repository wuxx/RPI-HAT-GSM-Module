# 树莓派驱动外设

## 开启i2c-0, i2c-1驱动
树莓派引出的40pin接口有两路i2c总线，配置的方式略微有一些不同。
### 开启i2c-0
开启i2c-0需要手动修改/boot/config.txt 配置脚本，在其中加入以下配置，重新启动即可开启i2c-0
```
dtparam=i2c0=on
```
重启之后，可使用以下命令进行确认i2c设备节点是否已创建
```
$ls /dev/i2c-*
/dev/i2c-0  /dev/i2c-1
```
### 开启i2c-1
可直接使用官方提供的raspi-config工具开启i2c-1
```
$sudo raspi-config
```
在选项中选择  *Interfacing Option -> I2C -> Yes* 即可配置开启i2c-1，配置即时生效，可使用以下命令进行确认
```
$ls /dev/i2c-*
/dev/i2c-0  /dev/i2c-1
```

### 实用工具
可使用i2cdetect工具对i2c从设备进行扫描
```
$i2cdetect -y 0 #扫描/dev/i2c-0
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- 44 -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- --
```

## 驱动1.44寸SPI LCD
## 驱动气压传感器BMP180
bmp180传感器可用于获取温度和大气压强，由于大气压强经过换算，可以计算出当前所处的海拔（当然由于气候季节原因，获取出的海拔值会存在一定偏差）  
wiringPi的命令行工具gpio已经集成了bmp180的驱动，可使用以下命令
```
$gpio -x bmp180:100 aread 100   # 读取温度
$gpio -x bmp180:100 aread 101   # 读取气压值，单位为毫巴
$gpio -x bmp180:100 aread 102   # 读取换算出的海拔

```
也可使用仓库中的python脚本进行驱动，只需执行命令
```
$python bmp180_example.py
```
即可获取温度，气压和海拔值并定时刷新，可查看源码并根据自己的具体需求进行二次修改。

## 驱动温湿度传感器SHT30
sht30为温湿度传感器，使用上相对比较简单，只需从I2C设备的固定地址处读出原始数值并做一个简单的换算即可得出温湿度值。
```
$python sht30.py
```
## 驱动PWM LED
底板上的GPIO.1引脚上接了一个LED，可实现PWM输出控制。  
请注意GPIO有两种命名规则，一种是BCM规则，这是底层SoC的GPIO编号，是有一定物理意义的，另一种是wPi规则，这是wiringPi库方便软件编程定义的编号，通常建议使用wiringPi规则的编号，方便命令行的控制和软件编程。
例程使用wiringPi库+C实现，实现了类似呼吸灯的效果
```
#include <wiringPi.h>
#include <softPwm.h>

#define LED    1

int main()
{
    int i;
    unsigned int duty = 0;

    wiringPiSetup();

    pinMode(LED, OUTPUT) ;

    for (i = 0; i < 5; i++) {

        if (i & 0x1) {
            digitalWrite(LED, HIGH);
        } else {
            digitalWrite(LED, LOW);
        }

        delay(1000);

    }

    pinMode(LED, SOFT_PWM_OUTPUT) ;
    softPwmCreate(LED, 0, 100); /* range 0 - 100 */

    while (1) {
        softPwmWrite(LED, (duty++) % 100);
        delay(30);
    }
    return 0;
}
```
在例程目录下输入
```
$make
$./pwm_led
```
即可编译出可执行文件并测试执行。


## 树莓派WiFi MQTT上报私有云服务器
本节与扩展板关系不大，说明如何使用树莓派自带的WiFi实现MQTT消息上报。
### 云服务器搭建
您需要一个公网的linux云服务器，可选择当前市面上流行的云服务提供商如阿里云、腾讯云、华为云等。
若云服务器只为测试使用，则成本考虑，选择最低配置机器即可。

#### MQTT服务端 mosquitto 安装
```
$sudo apt-get install mosquitto
$sudo apt-get install mosquitto-clients	#安装客户端，建议执行
```

#### MQTT服务端 mosquitto 配置
从安全性的角度，建议为MQTT连接配置密码，（一般服务均为开机自动启动，若无密码，则任何人可随意连接至MQTT服务端）
```
$sudo mosquitto_passwd -c /etc/mosquitto/pwfile mqtt_user
user: mqtt_user
passwd:123456789
```
#### MQTT服务端 订阅topic
只需要连接到mqtt服务器，任何角色均可订阅或者推送topic，这里为测试方便我们在服务端订阅topic，在树莓派端推送消息到topic，这样可在服务端收到消息
```
$sudo mosquitto_sub -h 123.45.67.89 -t "topic_test" -u mqtt_user -P 123456789
```
命令会挂起等待topic下的消息，当有消息到来时，会输出消息内容。

### 树莓派安装MQTT客户端
安装mqtt客户端
```
$sudo apt-get install mosquitto-clients
```

推送消息到topic
```
$sudo mosquitto_pub -h 123.45.67.89 -t "topic_test" -u mqtt_user -P 123456789 -m "test-msg"
```

当订阅者收到消息后，输出如下
```
$sudo mosquitto_sub -h 123.45.67.89 -t "topic_test" -u mqtt_user -P 123456789
test-msg
```
