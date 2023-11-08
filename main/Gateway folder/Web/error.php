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
		$IGN = $row['IGN'];
	}
	mysqli_close($con);

	$con = mysqli_connect("180.71.2.76:3393", "root", "ghdtjr1227!", "car_info");
        $query ="select * from error_code;";
        if(mysqli_connect_errno($con))
        {
                echo "MySQL 접속 실패:".mysqli_connect_error();
        }
        mysqli_set_charset($con,"utf8");
	$res = mysqli_query($con,$query);
        $code = array();
	$Danger = array();
	$Detail = array();
	$resolve = array();
	$cnt = 0;
        while($row = mysqli_fetch_array($res))
        {
		array_push($code, $row['code']);
		array_push($Danger, $row['Danger']);
		array_push($Detail, $row['Detail']);
		array_push($resolve, $row['res']);
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
		td {
			text-align:center;
		}
	</style>
    </head>
<body topmargin="0" leftmargin="0">
	<div id="choice">
		<ul>
		 <li><img src="Team_logo.jpg" alt="이미지 없음" style="height: 40px; width: auto;"></li>
		 <li><a href="http://180.71.2.76:3390\main.html" target="_self">Main</a></li>
		 <li><a href="http://180.71.2.76:3390\statistics.html" target="_self">그래프 통계</a></li>
		 <li><a href="http://180.71.2.76:3390\exchange.php" target="_self">차량 부품 교체 정보</a></li>
		 <li><a class="active" href="http://180.71.2.76:3390\error.php" target="_self">고장정보</a></li>
		</ul>
	</div>

	<div id="IGN">
		<h1>IGN : <a id="output"></a>
		<script>
			var x1 = <?php echo($IGN); ?>;
			if (x1 == 1)
				document.write("ON");
			else
				documnet.write("OFF");
			setInterval(function reload(){
				window.location.reload(true);
			},3000);
		</script>
		</h1>
	</div>

	<center>
		<fieldset style = "float:center; padding:10px; width:50%;">
			<legend style="font-size:2em;"> 고장코드 정보</legend>
			<table border="1" width="700" height="">
			        <thead>
			                <tr bgcolor="#FFF2FF">
			                        <th>고장코드</th>
			                        <th>고장 위험도</th>
						<th>고장 내용 상세</th>
						<th>해결방법</th>
			                </tr>
			        </thead>
				<tbody>
			                <tr bgcolor='#F2FFFF'>
						<td><?php $num=0; echo$code[$num]; ?></td>
						<td style="text-align:center;">
							<script>
								var dg = <?php echo$Danger[0]; ?>;
								if (dg == 3){
									document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
								}
								else if (dg == 2){
									document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
								}
								else
									document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
							</script>
						</td>
						<td><?php echo$Detail[0]; ?></td>
						<td><?php echo$resolve[0]; ?></td>
					</tr>
			                <tr bgcolor='#F2FFFF'>
						<td><?php echo$code[1]; ?></td>
						<td style="text-align:center;">
							<script>
								var dg = <?php echo$Danger[1]; ?>;
								if (dg == 3){
									document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
								}
								else if (dg == 2){
									document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
								}
								else
									document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
							</script>
						</td>
						<td><?php echo$Detail[1]; ?></td>
						<td><?php echo$resolve[1]; ?></td>
					</tr>
					<tr bgcolor='#F2FFFF'>
						<td><?php echo$code[2]; ?></td>
						<td style="text-align:center;">
							<script>
								var dg = <?php echo$Danger[2]; ?>;
								if (dg == 3){
									document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
								}
								else if (dg == 2){
									document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
								}
								else
									document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
							</script>
						</td>
						<td><?php echo$Detail[2]; ?></td>
						<td><?php echo$resolve[2]; ?></td>
					</tr>
					<tr bgcolor='#F2FFFF'>
						<td><?php echo$code[3]; ?></td>
						<td style="text-align:center;">
							<script>
								var dg = <?php echo$Danger[3]; ?>;
								if (dg == 3){
									document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
								}
								else if (dg == 2){
									document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
								}
								else
									document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
							</script>
						</td>
						<td><?php echo$Detail[3]; ?></td>
						<td><?php echo$resolve[3]; ?></td>
					</tr>
					<tr bgcolor='#F2FFFF'>
						<td><?php echo$code[0]; ?></td>
						<td style="text-align:center;">
							<script>
								var dg = <?php echo$Danger[0]; ?>;
								if (dg == 3){
									document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
								}
								else if (dg == 2){
									document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
								}
								else
									document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
							</script>
						</td>
						<td><?php echo$Detail[0]; ?></td>
						<td><?php echo$resolve[0]; ?></td>
					</tr>
					<tr bgcolor='#F2FFFF'>
						<td><?php echo$code[0]; ?></td>
						<td style="text-align:center;">
							<script>
								var dg = <?php echo$Danger[0]; ?>;
								if (dg == 3){
									document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
								}
								else if (dg == 2){
									document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
								}
								else
									document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
							</script>
						</td>
						<td><?php echo$Detail[0]; ?></td>
						<td><?php echo$resolve[0]; ?></td>
					</tr>
					<tr bgcolor='#F2FFFF'>
						<td><?php echo$code[0]; ?></td>
						<td style="text-align:center;">
							<script>
								var dg = <?php echo$Danger[0]; ?>;
								if (dg == 3){
									document.write("<button id='test_btn1'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
								}
								else if (dg == 2){
									document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2'>Middle</button>"+"<button id='test_btn3' style='opacity:0.3'>LOW</button>");
								}
								else
									document.write("<button id='test_btn1' style='opacity:0.3'>HIGH</button>"+"<button id='test_btn2' style='opacity:0.3'>Middle</button>"+"<button id='test_btn3'>LOW</button>");
							</script>
						</td>
						<td><?php echo$Detail[0]; ?></td>
						<td><?php echo$resolve[0]; ?></td>
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


