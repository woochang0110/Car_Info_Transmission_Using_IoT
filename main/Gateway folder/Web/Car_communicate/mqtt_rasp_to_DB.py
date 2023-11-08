import pymysql
import paho.mqtt.client as mqtt
import time

tmp = []

def on_message(client, user_data, message):
	global tmp
	tmp = (str(message.payload.decode("utf-8")))
	tmp = tmp.split(" ")
	print("MQTT_received message: ",tmp)
	DB_update()

def DB_update():
	# MySQL Connection
	conn = pymysql.connect(host='180.71.2.76',port=3393, user='root', password='ghdtjr1227!', db='car_info')
	cur = conn.cursor()
	sql = 'Insert Into Env_data(IGN,RPM,D,V,Eng_temp,Fuel_level) values(%s,%s,%s,%s,%s,%s)'
        cur.execute(sql, tmp)
        conn.commit()
        conn.close()
        print("DB_send_complete")


if __name__ == "__main__" :
	mqttBroker = "localhost"
	print("mqttBroker init")

	client = mqtt.Client("Esp_32")
	client.connect(mqttBroker)
	print("client connect OK...")

	client.loop_start()

	client.subscribe("Env_car")
	client.on_message = on_message
	try:
		while True:
			time.sleep(0.05)
	except:
		print("end")
