<?php
try {
    $options = [
        PDO::ATTR_EMULATE_PREPARES   => false,
        PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
        PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC
    ];
    $pdo = new PDO("mysql:host=$dbHost;dbname=$dbName;", $dbUser, $dbPass, $options);
} catch (PDOException $e) {
    die("An error occurred during database connection.");
}
?>