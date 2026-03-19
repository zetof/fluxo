<?php
function ComputeDewPoint($t, $h) {
	$a = 17.27;
	$b = 237.7;
	$alpha = $a * $t / ($b + $t) + log($h / 100);
	$dew = round($b * $alpha / ($a - $alpha));
	$delta = $t - $dew;
	if($delta < 2) $p1 = 'high_risk';
	elseif ($delta <= 5) $p1 = 'moderate_risk';
	else $p1 = 'no_risk';
	if ($dew < 0) $p2 = 'frost';
	elseif($dew <= 5) $p2 = 'dew';
	else $prompt = 'nothing';
    return ['val' => round($b * $alpha / ($a - $alpha), 1), 'desc' => _t('dewpoint', $p1)._t('dewpoint', $p2)];
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
    	$stmt = $pdo->prepare('SELECT * FROM measures WHERE sensor_id=? ORDER BY id DESC LIMIT 25');
    	$stmt->execute([$id]);
    	$rows = $stmt->fetchAll(PDO::FETCH_ASSOC);
    	foreach ($rows as $key=>$value) {
        	$t = $value['sensor_temp'];
        	$h = $value['sensor_humidity'];
        	$p = $value['sensor_pressure'];
      		switch($key){
        		case 0:
        			$time = $value['sensor_time'];
        			$valid = time() - strtotime($time) < 3600;
        			$last_t = $t;
        			$last_h = $h;
        			$last_p = $p;
        			$dewPoint = ComputeDewPoint($t, $h);
          			$last = ['temperature' => $t, 'humidity' => $h, 'pressure' => $p];
          			break;
        		case 3:  
          			$h_3 = ['temperature' => $t, 'humidity' => $h, 'pressure' => $p];
          			$delta_p = $last_p - $p;
      		}
      	}
      	switch(true) {
      		case($delta_p < -2):
      			$score = -3;
      			break;
      		case($delta_p < -1):
      			$score = -2;
      			break;
      		case($delta_p < 1):
      			$score = 0;
      			break;
      		case($delta_p < 2):
      			$score = 2;
      			break;
      		default:
      			$score = 3;
      	}
      	switch(true) {
      		case($h > 85):
      			$score += -2;
      			break;
      		case($h > 70):
      			$score += -1;
      			break;
      		case($h > 50):
      			$score += 0;
      			break;
      		default:
      			$score += 1;
      	}
      	switch(true) {
      		case($score <= -3):
      			$forecast = 'Pluie ou vent proche';
      			break;
      		case($score <= -1):
      			$forecast = 'Dégradation probable';
      			break;
      		case($score == 0):
      			$forecast = 'Stable';
      			break;
      		case($score >= 1):
      			$forecast = 'Amélioration';
      		case($score >= 4):
      			$forecast = 'Beau temps installé';
      	}
      	$data = ['valid' => $valid, 'time' => $time, 'last' => $last, 'h_3' => $h_3, 'dewPoint' => $dewPoint, 'score' => $score, 'forecast' => $forecast ];
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