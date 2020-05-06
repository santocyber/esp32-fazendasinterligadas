<html>
<head>
<title>Mirako I0T</title>
</head>
<body>

Mirako IoT

<?php

$client = new \Mosquitto\Client;
$client->setCredentials('mqtt', 'mqtt');
$client->onConnect('connect');
$client->onDisconnect('disconnect');
$client->onSubscribe('subscribe');
$client->onMessage('message');
$client->connect('iot.mirako.org');
$client->subscribe('#', 0);
 

while (true) {
        $client->loopForever();
//        sleep(2);
}
 
$client->disconnect();
unset($client);
 
function connect($r) {
        echo "I got code {$r}\n";
}
 
function subscribe() {
        echo "Subscribed to a topic\n";
}


function message($message) {
        printf("\nGot a message on topic %s with payload:%s",
                $message->topic, $message->payload);
}


function disconnect() {
        echo "Disconnected cleanly\n";
}
