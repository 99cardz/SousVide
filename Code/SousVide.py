from w1thermsensor import W1ThermSensor
import RPi.GPIO as GPIO
import time

sensor = W1ThermSensor()
temperature_in_celsius = sensor.get_temperature()
print(temperature_in_celsius)

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(18,GPIO.OUT)
GPIO.output(18,GPIO.HIGH)
time.sleep(1)
GPIO.output(18,GPIO.LOW)
target_temperature = int(input("target Temperature"))
while 1:
    current_temperature = sensor.get_temperature()
    print(current_temperature)
    if current_temperature < target_temperature:
        GPIO.output(18,GPIO.HIGH)
        print("ON")
    elif current_temperature >= target_temperature:
        GPIO.output(18,GPIO.LOW)
        print("OFF")
    time.sleep(1)
