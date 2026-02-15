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

$endpoints['sensors'] = function(array $requestData, int $id, PDO $pdo): void {
	if(isset($requestData['temperature']) && isset($requestData['humidity'])) {
		$temp = $requestData['temperature'];
		$humidity = $requestData['humidity'];
		$stmt=$pdo->prepare('INSERT INTO sensors(sensor_id, sensor_temp, sensor_humidity, sensor_time) VALUES(?, ?, ?, ?)');
		$stmt->execute([$id, $temp, $humidity, date('Y-m-d H:i:s')]);
		http_response_code(201);
		echo 'INSERTED';
	}
	else {
		http_response_code(400);
		echo 'Missing or wrong parameters for this endpoint';	
	}
};

if(!isset($_SERVER['HTTP_X_API_KEY'])) {
	http_response_code(403);
	die('You must provide a valid API key to proceed');
}

$header = $_SERVER['HTTP_X_API_KEY'];
$sep = strpos($header, '::');
if($sep === false) {
	http_response_code(403);
	die('Your API key is malformed');	
}

$keyName = substr($header, 0, $sep);
$key = substr($header, $sep + 2);

$stmt = $pdo->prepare('SELECT * FROM api_keys WHERE key_name=?');
$stmt->execute([$keyName]);
$apiKey = $stmt->fetch();
if($apiKey) {
	if(password_verify($key, $apiKey['key_hash'])) {
		$role = $apiKey['key_role'];
		$id = $apiKey['key_id'];
	}
	else {
		http_response_code(403);
		die('Your API key is not correct');		
	}	
}
else {
	http_response_code(403);
	die('Your API key does not exists');		
}

$parsedURI = parse_url($_SERVER['REQUEST_URI']);
$endpointName = str_replace($BASE_URI, '', $parsedURI['path']);
$method = $_SERVER['REQUEST_METHOD'];
$check = true;
switch ($method) {
    case 'GET':
    	$check = strpos($role, 'r');
        $requestData = $_GET;
        break;
    case 'POST':
    	$check = strpos($role, 'w');
        $requestData = json_decode(file_get_contents('php://input'), true);;
        break;
    default:
    	$check = false;
}
if($check === false) {
    http_response_code(401);
	die('Request method not allowed for your profile');	
}
if(isset($endpoints[$endpointName])) {
	$endpoints[$endpointName]($requestData, $id, $pdo);	
}
else {
	http_response_code(404);
	echo 'This endpoint does not exists';
}
?>