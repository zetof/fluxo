<?php
include('config.php');
include('i18n.php');
include('db_classes.php');
include('pdo.php');

ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

$BASE_URI = '/';
$endpoints = array();
$requestData = array();

function getAPI($endpoint, $token, $requestData) {
	$baseURL = 'https://fluxo.zetof.net/';
	$params = ['token' => $token];
	switch($endpoint) {
		case 'sensors':
			if(!isset($requestData['id'])) die('Missing sensor ID');
			$params['id'] = $requestData['id'];
			break;
	}
	$url = $baseURL.$endpoint.'?'.http_build_query($params);
	$curl = curl_init();
	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($curl, CURLOPT_FAILONERROR, true);
	$response = curl_exec($curl);
	curl_close($curl);
	if($response === false) die('An error occurred while requesting data');
	return json_decode($response, true);
}

$endpoints['sensors'] = function($method, $token, array $requestData, PDO $pdo): void {
	include('sensors_api.php');
};

$endpoints['measures'] = function($method, $token, array $requestData, PDO $pdo): void {
	$data = getAPI('sensors', $token, $requestData);
	include('gauges.php');
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
	$endpoints[$endpointName]($method, $token, $requestData, $pdo);	
}
else {
	http_response_code(404);
	echo 'This endpoint does not exists';
}
?>