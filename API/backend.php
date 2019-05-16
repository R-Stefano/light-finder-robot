<?php

if ($_POST['functionName']=="addRecord") {
    $deviceIP=$_POST['clientIP'];
    $number=$_POST['number'];
    $file = fopen('devices.txt', 'a');
    $textOnFile="IP: ". $deviceIP ." sended number: ".$number."\n";
    fwrite($file, $textOnFile);
    fclose($file); 
}

?>
