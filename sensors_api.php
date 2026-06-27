<?php
function computeDewPoint($t, $h) {
	$a = 17.27;
	$b = 237.7;
	$alpha = $a * $t / ($b + $t) + log($h / 100);
	return round($b * $alpha / ($a - $alpha), 1);
}

function computePrediction($values) {
	$p = $values[0]['pressure'];
	$p3 = $values[3]['pressure'];
	$p6 = $values[6]['pressure'];
	$p12 = $values[12]['pressure'];
	$p24 = $values[12]['pressure'];
	$dp3 = $p - $p3;
	$dp12 = $p - $p12;
	$dp24 = $p - $p24;
	$ap = $dp3 - ($p3 - $p6);
	$t = $values[0]['temperature'];
	$t3 = $values[3]['temperature'];
	$h = $values[0]['humidity'];
	$h3 = $values[3]['humidity'];
	$dt = $t - $t3;
	$td = computeDewPoint($t, $h);
	$td3 = computeDewPoint($t3, $h3);
	$dtd = $td - $td3;
	$imd = 1.2 * $dp3 + 0.8 * $dp12 + 0.4 * $dp24 +1.5* $ap + 0.7 * $dt + 1.0 * $dtd;
	$delta = round($t - $td, 1);
	if($t < 0 && $delta > 5) $p1 = 'black_frost';
	elseif ($t < 0 && $td < 0 && $delta <= 5) $p1 = 'frost';
	elseif($t >= 0 && $t < 2 && $td > 0 && $delta <= 3) $p1 = 'white_frost';
	elseif($t >= 0 && $delta < 2) $p1 = 'mist';
	elseif($t > 0 && $td > 0 && $delta >= 2 && $delta <= 5) $p1 = 'dew';
	elseif($delta > 5 && $delta <= 10) $p1 = 'humide';
	else $p1 = 'dry';
    return ['IMD' => $imd, 'DP3' => $dp3, 'DP12' => $dp12, 'DP24' => $dp24, 'AP' => $ap,'DT' => $dt, 'DTD' => $dtd,  'val' => $td, 'delta' => $delta, 'desc' => _t('dewpoint', [$p1])];
}

if(isset($requestData['id'])) {
	$id = $requestData['id'];
	$stmt = $pdo->prepare('SELECT * FROM sensors WHERE id=?');
	$stmt->execute([$id]);
	$sensor = $stmt->fetch();
	if($sensor) {
		$location = $sensor['location'];
	}
	else {
		http_response_code(400);
		die('Not existing sensor ID');
	}			
}
else {
	http_response_code(400);
	die('Missing sensor ID');			
}

switch($method) {
	case 'GET':
	$values = [];
    	$stmt = $pdo->prepare('SELECT * FROM measures WHERE sensor_time >= now() - INTERVAL 25 HOUR AND sensor_id=? ORDER BY id DESC');
    	$stmt->execute([$id]);
    	$rows = $stmt->fetchAll(PDO::FETCH_ASSOC);
    	foreach ($rows as $key=>$value) {
    		$time = $value['sensor_time'];
        	$temp = $value['sensor_temp'];
        	$humi = $value['sensor_humidity'];
        	$pres = $value['sensor_pressure'];
        	array_push($values, ['time' => $time, 'temperature' => $temp, 'humidity' => $humi, 'pressure' => $pres]);
      	}
        $valid = time() - strtotime($values[0]['time']) < 3600;
        $prediction = ComputePrediction($values);
      	$data = ['location' => $location, 'valid' => $valid, 'prediction' => $prediction, 'values' => $values];
      	header('Content-Type: application/json; charset=utf-8');
      	echo json_encode($data, JSON_UNESCAPED_UNICODE);
		break;
	case 'POST':
		if(isset($requestData['temperature']) && isset($requestData['humidity']) && isset($requestData['pressure'])) {
			$temp = $requestData['temperature'];
			$humidity = $requestData['humidity'];
			$pressure = $requestData['pressure'];
			$stmt=$pdo->prepare('INSERT INTO measures(sensor_id, sensor_temp, sensor_humidity, sensor_pressure, sensor_time) VALUES(?, ?, ?, ?, ?)');
			$stmt->execute([$id, $temp, $humidity, $pressure, date('Y-m-d H:i:s')]);
			http_response_code(201);
			echo 'INSERTED';
		}
	else {
		http_response_code(400);
		echo 'Missing or wrong parameters for this endpoint';			
	}
}
?>