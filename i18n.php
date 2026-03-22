<?php
$lang = 'fr';
$langPath = __DIR__ . "/i18n/lang.{$lang}";

if (!file_exists($langPath)) {
    die('I18N file not found.');
}

$i18n = parse_ini_file($langPath, true);

function _t($section, array $prompts, $display=false, $up=false, $down=false) {
    global $i18n;
    $constructed = '';
    foreach($prompts as $prompt) {
        if(isset($i18n[$section][$prompt])) $constructed .= $i18n[$section][$prompt];
        else $constructed .= "__i18n__<{$section}><{$prompt}>";
    }
    if($up) $constructed = mb_strtoupper($constructed, 'UTF-8');
    if($down) $constructed = strtolower($constructed);
    if($display) echo $constructed;
    else return $constructed;
}

function _d($datetime, $display=false) {
    global $i18n;
    $timeStamp = date_create($datetime);
    $formatedDate = date_format($timeStamp, $i18n['datetime']['format1']);
    $formatedDate .= $i18n['datetime'][strtolower(date_format($timeStamp, $i18n['datetime']['format2']))];
    $formatedDate .= date_format($timeStamp, $i18n['datetime']['format3']);
    if($display) echo $formatedDate;
    else return $formatedDate;
}