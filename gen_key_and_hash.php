<?php
	
	function generateApiKey(int $length=32): string {
		$bytes = random_bytes($length);
		return rtrim(strtr(base64_encode($bytes), '+/', '-_'), '=');
	}

	$apiKey = generateApiKey();
	$hash = password_hash($apiKey, PASSWORD_BCRYPT);
	echo "API-KEY: ".$apiKey."</br>";
	echo "HASH: ".$hash."</br>";

	if(password_verify($apiKey, $hash)) {
		echo "OK ";
	}
	else {
		echo "KO";
	}