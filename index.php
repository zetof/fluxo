<?php
include_once('config.php');
include_once('db_classes.php');
include_once('pdo.php');

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

$BASE_URI = '/';
$endpoints = array();
$requestData = array();

$endpoints['sensors'] = function($method, array $requestData, PDO $pdo): void {
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
    		$stmt = $pdo->prepare('SELECT * FROM measures WHERE sensor_id=? ORDER BY id DESC LIMIT 24');
    		$stmt->execute([$id]);
    		$rows = $stmt->fetchAll(PDO::FETCH_ASSOC);
    		foreach ($rows as $key=>$value) {
      			switch($key){
        			case 0:
          				echo 'var p_now = '.$value['sensor_pressure'].';';
          				echo 'var h_now = '.$value['sensor_humidity'].';';
          				echo 'var t_now = '.$value['sensor_temp'].';';
          				break;
        			case 3:  
          				echo 'var p_old = '.$value['sensor_pressure'].';';
          				echo 'var h_old = '.$value['sensor_humidity'].';';
          				echo 'var t_old = '.$value['sensor_temp'].';';
      			}
      		}
      		$data = [ 'location' => $location ];
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
};

$endpoints['measures'] = function($method, array $requestData, PDO $pdo): void {
	include_once('gauges.php');
};

$method = $_SERVER['REQUEST_METHOD'];
switch ($method) {
    case 'GET':
        $requestData = $_GET;
        if(isset($_GET['token'])) {
			$token = $_GET['token'];
		}
        break;
    case 'POST':
        $requestData = json_decode(file_get_contents('php://input'), true);
        if($requestData == null) {
        	$requestData = [];
        }
		if(isset($_SERVER['HTTP_X_API_KEY'])) {
			$token = $_SERVER['HTTP_X_API_KEY'];
		}
		break;
	default:
		$token = null;
}

if(!isset($token)) {
	http_response_code(403);
	die('You must provide a valid API key to proceed');	
}

$role = false;
$stmt = $pdo->query('SELECT * FROM api_keys');
$apiKeys = $stmt->fetchAll();
foreach ($apiKeys as $apiKey) {
	if(password_verify($token, $apiKey['key_hash'])) {
		$role = $apiKey['key_role'];
		break;
	}
}
if($role == false) {
	http_response_code(403);
	die('Your API key does not exists');		
}

switch ($method) {
    case 'GET':
    	$check = strpos($role, 'r');
        break;
    case 'POST':
    	$check = strpos($role, 'w');
        break;
    default:
    	$check = false;
}
if($check === false) {
    http_response_code(401);
	die('Request method not allowed for your profile: '.$role);	
}

$parsedURI = parse_url($_SERVER['REQUEST_URI']);
$endpointName = str_replace($BASE_URI, '', $parsedURI['path']);
if(isset($endpoints[$endpointName])) {
	$endpoints[$endpointName]($method, $requestData, $pdo);	
}
else {
	http_response_code(404);
	echo 'This endpoint does not exists';
}
?>