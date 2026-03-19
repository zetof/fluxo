<?php
$lang = 'fr';
$langPath = __DIR__ . "/i18n/lang.{$lang}";

if (!file_exists($langPath)) {
    die('I18N file not found.');
}

$i18n = parse_ini_file($langPath, true);

function _t($section, $prompt) {
    global $i18n;
    if(isset($i18n[$section][$prompt])) {
        return $i18n[$section][$prompt];
    }
    else {
        return "__i18n__<{$section}><{$prompt}>";
    }
}