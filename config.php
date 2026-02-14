<?php
$configPath = __DIR__ . '/config.ini';

if (!file_exists($configPath)) {
    die("Configuration file not found.");
}

$config = parse_ini_file($configPath, true);

$dbHost = $config['database']['host'];
$dbName = $config['database']['dbname'];
$dbUser = $config['database']['username'];
$dbPass = $config['database']['password'];
?>