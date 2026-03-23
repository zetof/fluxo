<?php
function computeDewPoint($values) {
	$a = 17.27;
	$b = 237.7;
	$h = $values[0]['humidity'];
	$t = $values[0]['temperature'];
	$alpha = $a * $t / ($b + $t) + log($h / 100);
	$dew = round($b * $alpha / ($a - $alpha), 1);
	$delta = round($t - $dew, 1);
	if($delta < 2) $p1 = 'high_risk';
	elseif ($delta <= 5) $p1 = 'moderate_risk';
	else $p1 = 'no_risk';
	if ($dew < 0) $p2 = 'frost';
	elseif($dew <= 5) $p2 = 'dew';
	else $p2 = 'nothing';
    return ['val' => $dew, 'delta' => $delta, 'desc' => _t('dewpoint', [$p1, $p2])];
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
    	$stmt = $pdo->prepare('SELECT * FROM measures WHERE sensor_time >= now() - INTERVAL 24 HOUR AND sensor_id=? ORDER BY id DESC');
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
        $dewPoint = ComputeDewPoint($values);
      	$data = ['location' => $location, 'valid' => $valid, 'dewPoint' => $dewPoint, 'values' => $values];
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