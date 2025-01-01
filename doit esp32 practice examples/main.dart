import 'package:flutter/material.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';
import 'package:uuid/uuid.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Seat Status App',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const SeatStatusScreen(),
    );
  }
}

class SeatStatusScreen extends StatefulWidget {
  const SeatStatusScreen({Key? key}) : super(key: key);

  @override
  _SeatStatusScreenState createState() => _SeatStatusScreenState();
}

class _SeatStatusScreenState extends State<SeatStatusScreen> {


  var uuid = Uuid();
  late String clientId; // Unique UUID for each session

  late final MqttServerClient client; // Declare client without initialization

  String status = "Disconnected"; // Status message
  String seatStatus = "Unknown"; // Received status (on/off)
  bool isConnected = false; // Flag to track connection status

  @override
  void initState() {
    super.initState();
    clientId = 'FlutterClient_${uuid.v4()}'; // Initialize clientId
    client = MqttServerClient('broker.hivemq.com', clientId); // Initialize client
    setupMQTT(); // Initialize MQTT on startup
  }

  // MQTT Setup
  void setupMQTT() async {
    client.port = 1883; // Default MQTT port
    client.keepAlivePeriod = 30;
    client.logging(on: true); // Enable logs for debugging
    client.onConnected = onConnected;
    client.onDisconnected = onDisconnected;
    client.onSubscribed = onSubscribed;

    final connMessage = MqttConnectMessage()
        .withClientIdentifier('seat_status_client')
        .startClean(); // Clean session

    client.connectionMessage = connMessage;

    try {
      await client.connect(); // Connect to broker
    } catch (e) {
      print('Connection Error: $e');
      setState(() {
        status = 'Connection Failed';
        isConnected = false;
      });
    }

    if (client.connectionStatus!.state == MqttConnectionState.connected) {
      setState(() {
        status = "Connected";
        isConnected = true;
      });
      subscribeToTopic(); // Subscribe once connected
    } else {
      setState(() {
        status = "Connection Failed";
        isConnected = false;
      });
    }
  }

  // Publish Status
  void publishStatus(String status) {
    if (isConnected) {
      final builder = MqttClientPayloadBuilder();
      builder.addString(status);

      client.publishMessage('seat/1/status', MqttQos.atLeastOnce, builder.payload!);
    }
  }

  // Subscribe to Topic
  void subscribeToTopic() {
    client.subscribe('seat/1/status', MqttQos.atLeastOnce);
    client.updates!.listen((List<MqttReceivedMessage<MqttMessage>> c) {
      final recMessage = c[0].payload as MqttPublishMessage;
      final message =
          MqttPublishPayload.bytesToStringAsString(recMessage.payload.message);

      setState(() {
        seatStatus = message; // Update received status
      });

      print('Received: $message');
    });
  }

  // MQTT Event Handlers
  void onConnected() {
    print('Connected!');
    setState(() {
      isConnected = true;
    });
  }

  void onDisconnected() {
    print('Disconnected!');
    setState(() {
      isConnected = false;
    });
  }

  void onSubscribed(String topic) {
    print('Subscribed to $topic');
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Monitor'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text(
              'MQTT Status: $status',
              style: const TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 20),
            Text(
              'Status: $seatStatus',
              style: const TextStyle(fontSize: 24, fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 40),
            ElevatedButton(
              onPressed: isConnected
                  ? () {
                      publishStatus("on"); // Publish "on" status
                    }
                  : null,
              child: const Text('Send ON'),
            ),
            ElevatedButton(
              onPressed: isConnected
                  ? () {
                      publishStatus("off"); // Publish "off" status
                    }
                  : null,
              child: const Text('Send OFF'),
            ),
          ],
        ),
      ),
    );
  }
}
