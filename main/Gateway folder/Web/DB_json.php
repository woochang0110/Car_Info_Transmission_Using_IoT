<?php
	$index = $_GET['index'];
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
	// 배열형식의 결과를 json으로 변환 
	$json =  json_encode(array("DT"=>$DT,
				"IGN"=>$IGN,
				"RPM"=>$RPM,
				"D"=>$D,
				"V"=>$V,
				"Eng_temp"=>$Eng_temp,
				"Fuel_level"=>$Fuel_level),JSON_UNESCAPED_UNICODE);
	$bytes = file_put_contents("test.json", $json);
	mysqli_close($con);
	echo($json);
?>
