<?php
        $con = mysqli_connect("180.71.2.76:3393", "root", "ghdtjr1227!", "car_info");
        $query ="select * from avg_data order by DT DESC limit 7;";
        $query2 = "Select DATE_FORMAT(DT,'%Y-%m-%d') AS day, SUM(up_acel), SUM(down_acel) From stat_data group by day order by day DESC limit 7;";
        if(mysqli_connect_errno($con))
        {
                echo "MySQL 접속 실패:".mysqli_connect_error();
        }
        mysqli_set_charset($con,"utf8");
        $res = mysqli_query($con,$query);
        $res2 = mysqli_query($con,$query2);
        $DT = array();
        $DT_stat = array();
        $avg_rpm = array();
        $avg_speed = array();
        $avg_Distance = array();
        $avg_Eng_temp = array();
        $avg_Fuel = array();
        $up_count = array();
        $down_count = array();
        $count = 6;
        while($row = mysqli_fetch_array($res))
        {
                $IGN = $row['IGN'];
                $DT[$count] = strtotime($row['DT']);
                $DT[$count] = date("m.d",$DT[$count]);
                $avg_rpm[$count] = $row['avg_rpm'];
                $avg_speed[$count] = $row['avg_speed'];
                $avg_Distance[$count] = $row['avg_Distance'];
                $avg_Eng_temp[$count] = $row['avg_Eng_temp'];
                $avg_Fuel[$count] = $row['avg_Fuel'];
                $count = $count - 1;
        }
        $count = 6;
        while($row = mysqli_fetch_array($res2))
        {
                $DT_stat[$count] = strtotime($row['day']);
                $DT_stat[$count] = date("m.d",$DT_stat[$count]);
                $up_count[$count] = $row['SUM(up_acel)'];
                $down_count[$count] = $row['SUM(down_acel)'];
                $count = $count - 1;
        }
	// 배열형식의 결과를 json으로 변환
        $json =  json_encode(array(
		"DT"=>$DT,
		"IGN"=>$IGN,
		"ar"=>$avg_rpm,
		"av"=>$avg_speed,
		"ad"=>$avg_Distance,
		"aet"=>$avg_Eng_temp,
		"af"=>$avg_Fuel,
		"DT_stat"=>$DT_stat,
		"up_cnt"=>$up_count,
		"down_cnt"=>$down_count),JSON_UNESCAPED_UNICODE);
        $bytes = file_put_contents("test.json", $json);
        mysqli_close($con);
        echo($json);
?>
