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
	if(isset($requestData['temperature']) && isset($requestData['humidity']) && isset($requestData['pressure'])) {
		$temp = $requestData['temperature'];
		$humidity = $requestData['humidity'];
		$pressure = $requestData['pressure'];
		$stmt=$pdo->prepare('INSERT INTO sensors(sensor_id, sensor_temp, sensor_humidity, sensor_pressure, sensor_time) VALUES(?, ?, ?, ?, ?)');
		$stmt->execute([$id, $temp, $humidity, $pressure, date('Y-m-d H:i:s')]);
		http_response_code(201);
		echo 'INSERTED';
	}
	else {
		http_response_code(400);
		echo 'Missing or wrong parameters for this endpoint';	
	}
};

$endpoints['measures'] = function(array $requestData, int $id, PDO $pdo): void {
		#$stmt=$pdo->prepare('INSERT INTO sensors(sensor_id, sensor_temp, sensor_humidity, sensor_pressure, sensor_time) VALUES(?, ?, ?, ?, ?)');
		#$stmt->execute([$id, $temp, $humidity, $pressure, date('Y-m-d H:i:s')]);
		#http_response_code(201);
		#include_once('measures.php');
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
        $requestData = json_decode(file_get_contents('php://input'), true);;
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

$sep = strpos($token, '::');
if($sep === false) {
	http_response_code(403);
	die('Your API key is malformed');	
}

$keyName = substr($token, 0, $sep);
$key = substr($token, $sep + 2);

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
	$endpoints[$endpointName]($requestData, $id, $pdo);	
}
else {
	http_response_code(404);
	echo 'This endpoint does not exists';
}
?><!DOCTYPE html>