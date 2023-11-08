<?php
        $con = mysqli_connect("180.71.2.76:3393", "root", "ghdtjr1227!", "car_info");
        $query ="select * from Env_data order by DT Desc limit 1;";
        if(mysqli_connect_errno($con))
        {
                echo "MySQL 접속 실패:".mysqli_connect_error();
        }
        mysqli_set_charset($con,"utf8");
        $res = mysqli_query($con,$query);
        $result = array();
        while($row = mysqli_fetch_array($res))
        {
		$DT = $row['DT'];
		$IGN = $row['IGN'];
		$RPM = $row['RPM'];
		$D = $row['D'];
		$V = $row['V'];
		$Eng_temp = $row['Eng_temp'];
		$Fuel_level = $row['Fuel_level'];
	}
        mysqli_close($con);
?>

<!DOCTYPE html> 
<html>
<head>
<meta charset="UTF-8">
	<title>차량 정보</title>
	<script src="https://cdn.jsdelivr.net/npm/chart.js@2.8.0"></script>
	<style>
		td {
			text-align:center;
		}
		div.mar {
			margin-top: -40px;
			margin-left: 160px;
		}
		div.map {
			margin-top: 10px;
		}
	</style>
	<style type="text/css">
		ul {
			list-style-type: none;
			margin: 0;
			padding: 0;
			background-color: #333;
		}
		ul:after{
			content:'';
			display: block;
			clear:both;
		}
		li {
			float: left;
		}
		li a {
			display: block;
			color: white;
			text-align: center;
			padding: 14px 16px;
			text-decoration: none;
		}
		li a:hover:not(.active) {
			background-color: #111;
		}
		.active {
			background-color: #4CAF50;
		}
		#component {
			float:center;
			padding:10px;
		}
	</style>
	<style>
		#test_btn1{ 
			border-radius: 10px; 
			margin-right:10px; 
			border: 1px solid red; 
			background-color: red; 
			color: black; 
			padding: 5px; 
		} 
		#test_btn2{ 
			border-radius: 10px; 
			margin-right:10px; 
			border: 1px solid Yellow; 
			background-color: Yellow; 
			color: black; 
			padding: 5px; 
		} 
		#test_btn3{ 
			border-radius: 10px; 
			margin-right:10px;
			border: 1px solid skyblue; 
			background-color: skyblue; 
			color: black; 
			padding: 5px;  
		}
		#btn_group button:hover{ 
			color:white; 
			background-color: skyblue; 
		}
	</style>
    </head>
<body topmargin="0" leftmargin="0">
<div id="choice">
	<ul>
	 <li><img src="Team_logo.jpg" alt="이미지 없음" style="height: 40px; width: auto;"></li>
	 <li><a href="http://180.71.2.76:3390\main.html" target="_self">Main</a></li>
	 <li><a href="http://180.71.2.76:3390\statistics.html" target="_self">그래프 통계</a></li>
	 <li><a class="active" href="http://180.71.2.76:3390\exchange.php" target="_self">차량 부품 교체 정보</a></li>
	 <li><a href="http://180.71.2.76:3390\error.php" target="_self">고장정보</a></li>
	</ul>
</div>

<div id="IGN">
	<h1>IGN : <a id="output"></a>
		<script type="text/javascript">
			var x1 = <?php echo($IGN);?>;
			if (x1 == 1)
				document.write("ON");
			else
				document.write("OFF");
			setInterval(function reload(){
				window.location.reload(true);
			},3000);
		</script>
	</h1>
</div>

<center>
	<fieldset style = "float:center; padding:10px; width:50%;">
		<legend style="font-size:2em;"> 부품 교체 정보</legend>
		<table border="1" width="500" height="">
		        <thead>
		                <tr bgcolor="#FFF2FF">
		                        <th>교체 장비명</th>
		                        <th>교체 위험도</th>
		                </tr>
		        </thead>
			<tbody>
		                <tr bgcolor="#F2FFFF">
					<td>엔진오일</td>
					<!-- 5000~7000 -->
					<td>
						<script>
							var D = <?php echo$D; ?>;
							if (D >= 5000){
								document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else if (D >= 2500 && D <5000){
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
						</script>
					</td>
				</tr>
				<tr bgcolor="#F2FFFF">
					<td>오토미션오일</td>
					<!-- 30000~40000 -->
					<td>
						<script>
							var D = <?php echo$D; ?>;
							if (D >= 30000){
								document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else if (D >= 15000 && D <30000){
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
						</script>
					</td>
				</tr>
				<tr bgcolor="#F2FFFF">
					<td>파워오일</td>
					<!-- 40000~50000 -->
					<td>
						<script>
							var D = <?php echo$D; ?>;
							if (D >= 40000){
								document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else if (D >= 20000 && D <40000){
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
						</script>
					</td>
				</tr>
				<tr bgcolor="#F2FFFF">
					<td>브레이크 오일</td>
					<!-- 20000 or 30000~40000 -->
					<td>
						<script>
							var D = <?php echo$D; ?>;
							if (D >= 30000){
								document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else if (D >= 15000 && D <30000){
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
						</script>
					</td>
				</tr>
				<tr bgcolor="#F2FFFF">
					<td>와이퍼</td>
					<!-- 8000 or 6 month -->
					<td>
						<script>
							var D = <?php echo$D; ?>;
							if (D >= 8000){
								document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else if (D >= 4000 && D <8000){
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
						</script>
					</td>
				</tr>
				<tr bgcolor="#F2FFFF">
					<td>에어컨 필터</td>
					<!-- 9000~10000 -->
					<td>
						<script>
							var D = <?php echo$D; ?>;
							if (D >= 9000){
								document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else if (D >= 4500 && D < 9000){
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
						</script>
					</td>
				</tr>
				<tr bgcolor="#F2FFFF">
					<td>타이어</br>(한번 교체이후 마모상태에 따라 교체)</td>
					<!-- 9000 and 50000 -->
					<td>
						<script>
							var D = <?php echo$D; ?>;
							if (D >= 9000){
								document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else if (D >= 4500 && D < 9000){
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
						</script>
					</td>
				</tr>
				<tr bgcolor="#F2FFFF">
					<td>냉각수(부동액)</td>
					<!-- 2years or 40000 -->
					<td>
						<script>
							var D = <?php echo$D; ?>;
							if (D >= 40000){
								document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else if (D >= 10000 && D < 40000){
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
						</script>
					</td>
				</tr>
				<tr bgcolor="#F2FFFF">
					<td>배터리</td>
					<!-- 2~3years or 100000 -->
					<td>
						<script>
							var D = <?php echo$D; ?>;
							if (D >= 10000){
								document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else if (D >= 5000 && D < 10000){
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
						</script>
					</td>
				</tr>
				<tr bgcolor="#F2FFFF">
					<td>구동 벨트</td>
					<!-- 20000 or 30000~40000 -->
					<td>
						<script>
							var D = <?php echo$D; ?>;
							if (D >= 20000){
								document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else if (D >= 10000 && D < 20000){
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
						</script>
					</td>
				</tr>
				<tr bgcolor="#F2FFFF">
					<td>타이밍 벨트</td>
					<!-- 60000~80000 -->
					<td>
						<script>
							var D = <?php echo$D; ?>;
							if (D >= 60000){
								document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else if (D >= 30000 && D < 60000){
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
							}
							else
								document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
						</script>
					</td>
				</tr>
			</tbody>
		</table>
	</fieldset>
</center>
<hr>
<p style="font-size:0.8em; line-height:50%;">제작팀:CAN</p>
<p style="font-size:0.8em; line-height:50%;">제작자:정홍석</p>
</body>
</html>
