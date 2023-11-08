import pymysql
import time
from datetime import datetime

# MySQL Connection 연결
conn = pymysql.connect(host='180.71.2.76',port=3393, user='root', password='ghdtjr1227!', db='car_info')
#sql = 'select `RPM`,`D`,`V`,`Eng_temp`,`Fuel_level` from Env_data'
cur = conn.cursor()

def up_down_vel():
	up_count = 0
	down_count = 0
	sql = 'select DT,V from Env_data order by DT desc limit 2'
	cur.execute(sql)
	data = list(cur.fetchall())
	#conn.commit()
	print(data[0][1],data[1][1])
	first = data[0][1]
	second = data[1][1]
	if(first > second):
		V_diff = first - second
		print("V_diff is ",V_diff)
		if V_diff >= 10 :
			up_count = 1
		else :
			up_count = 0
	elif(first < second):
		V_diff = second - first
		print("V_diff is ",V_diff)
		if V_diff >= 10 :
			down_count = 1
		else :
			down_count = 0
	else:
		pass
	print("up_count is ",up_count,"down_count is ",down_count)
	#sql = 'insert into stat_data(Env_data_id, up_acel, down_acel) values(%s,%s,%s)'
	#cur.execute(sql,('1', up_count, down_count))
	#conn.commit()

if __name__ == "__main__" :
	while(1):
		# 급가속, 급감속
		up_down_vel()
		now = datetime.now()
		if now.hour == 23 and now.minute == 59 and now.second == 59:
			sql = 'Insert Into avg_data(Env_data_id,avg_rpm,avg_speed,avg_Distance,avg_Eng_temp,avg_Fuel) select 1,avg(RPM),avg(V),avg(D),avg(Eng_temp),avg(Fuel_level) From Env_data'
			cur.execute(sql)
			conn.commit()

			sql = 'Delete From Env_data'
			cur.execute(sql)
			conn.commit()

			conn.close()
			print("complete")
		else :
			pass
		time.sleep(1)
