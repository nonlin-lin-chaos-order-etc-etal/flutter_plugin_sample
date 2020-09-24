import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:flutter_crash_sample_plugin/flutter_crash_sample_plugin.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _sample = 'Unknown';

  @override
  void initState() {
    super.initState();
    initSample();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initSample() async {
    String sample;
    // Platform messages may fail, so we use a try/catch PlatformException.
    try {
      print("calling FlutterCrashSamplePlugin.sampleMethod()...");
      sample = await FlutterCrashSamplePlugin.sampleMethod();
      print("called FlutterCrashSamplePlugin.sampleMethod()");
    } on PlatformException {
      print("FlutterCrashSamplePlugin.sampleMethod() call failed");
      sample = 'PlatformException while sampleMethod().';
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {
      _sample = sample;
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: Text('sampleMethod() return value: $_sample'),
        ),
      ),
    );
  }
}
